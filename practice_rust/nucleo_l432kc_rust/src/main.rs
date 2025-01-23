/*
 * @file main.rs
 * @brief This file contains the main function and the CLI functions.
 * @details The main function initializes the hardware and starts the CLI service.
 *          The CLI service reads commands from the serial receiver and executes the corresponding functions.
 *          The CLI functions control the state of the LED and the stepper motor based on the received commands.
 * @author Sungsu Kim
 * @date 2025-01-11
 * @version 1.1.0
 */

#![no_main]
#![no_std]

use core::cell::RefCell;
use core::panic::PanicInfo;
use core::borrow::{Borrow, BorrowMut};
use core::ops::Deref;
use cortex_m_rt::entry;
use stm32l4xx_hal as hal;
use hal::pac::{Peripherals, USART2, interrupt, Interrupt};
use hal::gpio::gpioa::{PA0, PA4, PA5, PA6, PA7};
use hal::gpio::gpiob::{PB0, PB3};
use hal::gpio::{Analog, Input, Output, PushPull, PullDown, Edge};
use hal::adc::ADC;
use hal::delay::Delay;
use hal::serial::{Serial, Tx};
use hal::prelude::*;
use hal::time::MonoTimer;
use heapless::{String, Vec};
use spin::Mutex;

use my_nucleo_l432::print;
use my_nucleo_l432::serial_sender::SerialSender;
use my_nucleo_l432::stepper::Stepper;
use my_nucleo_l432::serial_sender::SENDER;

const SIZE_RX_BUFFER: usize = 64;
const MAX_ARGS: usize = 20;

type FixedStringSlices = Vec<String<SIZE_RX_BUFFER>, MAX_ARGS>;
type SerialSenderType = SerialSender<Tx<USART2>>;

static mut EXTI0_COUNT: u32 = 0;

//********************************************
// mutex testing...
pub struct TestStruct {
    value: u32,
}

static mut TEST: Mutex<u32> = Mutex::new(0u32);
static mut TEST_STRUCT: Mutex<TestStruct> = Mutex::new(TestStruct { value: 0 });
static mut TEST2: Mutex<RefCell<u32>> = Mutex::new(RefCell::new(0u32));
//static SENDER: Mutex<Option<SerialSenderType>> = Mutex::new(None);

pub struct Data {
    value: u32,
}
static GLOBAL_DATA: Mutex<RefCell<Option<Data>>> = Mutex::new(RefCell::new(None));
static GLOBAL_DATA2: Mutex<Option<Data>> = Mutex::new(None);
//********************************************

#[entry]

/**
 * Main function.
 */
fn main() -> ! {
    let (timer, mut led3, uart_tx, mut uart_rx, pa4, pa5, pa6, pa7, mut ain0, mut pa0, pb0) = init_hardware();

    let mut sender = SerialSender::new(uart_tx);
    my_nucleo_l432::serial_sender::connect(sender);

    let mut str_buffer: String<SIZE_RX_BUFFER> = String::new();
    str_buffer.clear();

    let ticks_per_ms = timer.frequency().to_Hz() / 1000;
    let ms_to_ticks = |ms: u32| -> u32 { ticks_per_ms * ms };
    let mut led_tick = timer.now();
    let mut adc_tick = timer.now();

    let mut stepper = Stepper::new(pa4, pa5, pa6, pa7, timer.clone() );
    let mut monitor_adc = false;
    let mut interval_adc_mon = 500u32;

    let mut exti0_count = 0u32;

    //********************************************
    // mutex testing...
    //*SENDER.borrow().borrow_mut() = &Mutex::new(RefCell::new(Some(sender)));
    // SENDER.get_mut().borrow().borrow_mut().as_mut().unwrap().send_bytes("Hello, world!\r\n");
    //let sender_ref = SENDER.lock();
    //let sender_ = sender_ref.borrow().as_ref().unwrap();
    //print!(sender_, "Hello, world!\r\n");
    //********************************************

    // start the service
    cli_clear_screen();
    //print!(sender, "\x1b[96m");  // set color to cyan
    cli_print_info();
    print!("CLI: Enter a command...\r\n");

    //********************************************
    // mutex testing...
    unsafe {
        *TEST.lock() += 1;        
        //print!(sender, "Test mutex: {}\r\n", *TEST.lock() );

        *TEST.lock() += 1;        
        //print!(sender, "Test mutex: {}\r\n", *TEST.lock() );

        let mut obj = TEST_STRUCT.lock();
        obj.value = 3;

        let data = Data{value: 42}; 
        GLOBAL_DATA.lock().replace(Some(data));
        let guard = GLOBAL_DATA.lock(); 
        // let data_ref = &guard.borrow().
        // if let Some(data) = data_ref {        
        // }

        let data2 = Data{value: 45};
        GLOBAL_DATA2.lock().replace(data2);
        GLOBAL_DATA2.lock().as_mut().unwrap().value = 50;
    }
    //********************************************

    loop {
        if led_tick.elapsed() > ms_to_ticks(1000) {
            led_tick = timer.now();
            led3.toggle();            
        }

        if monitor_adc && adc_tick.elapsed() > ms_to_ticks(interval_adc_mon) {
            adc_tick = timer.now();
            let value = ain0.read(&mut pa0).unwrap_or(0u16);
            let volts = (value as f32) * 3.3f32 / ain0.get_max_value() as f32;
            print!("ADC: {}[cnt], {:.3}[v]\r\n", value, volts);
        }

        if unsafe { EXTI0_COUNT } != exti0_count {
            exti0_count = unsafe { EXTI0_COUNT };
            print!("EXTI0: {}, pin={}\r\n", exti0_count, pb0.is_high());
        }

        stepper.run_task();

        if let Some(slices) = get_command_slices(&mut uart_rx, &mut str_buffer) {
            if let Some( command ) = slices.get(0) {
                print!("CLI: #args={}\r\n", slices.len());
                match command.as_str() {
                    "info"      => { cli_print_info(); }
                    "led"       => { cli_led(slices.clone(), &mut led3); }
                    "stepper"   => { cli_stepper(slices.clone(), &mut stepper); }
                    "adc"       => { cli_adc(slices.clone(), &mut monitor_adc, &mut interval_adc_mon); }
                    "cls"       => { cli_clear_screen(); }
                    "clear"     => { cli_clear_screen(); }
                    _           => { print!("CLI: undefined command ({})\r\n", command.as_str());}
                }
            } else {
                if monitor_adc {
                    monitor_adc = false;
                    print!("ADC: monitor stopped\r\n");
                }
                if stepper.is_moving() {
                    stepper.stop_and_reset();
                }
            }
        }
    }

    loop {
    }
}

/**
 * This function clears the screen by sending the ANSI escape sequence.
 */
fn cli_clear_screen() {
    print!("\x1b[H\x1b[2J");
}

/**
 * This function prints the welcome message and project information.
 */
fn cli_print_info() {
    print!("\r\n");
    print!("************************************\r\n");
    print!("* Welcome to STM32L431 Rust Project\r\n");
    print!("* Version: 1.1.0\r\n");
    print!("* Author: SSKIM \r\n");
    print!("************************************\r\n");
}

/**
 * This function controls the state of an LED based on the command received.
 * The command is expected to be in the first slice of the `slices` vector.
 * If the command is "on", the LED is set to high. Otherwise, the LED is set to low.
 *
 * @param slices: A vector of `String<SIZE_RX_BUFFER>`, each representing a slice of the received command.
 * @param led: A mutable reference to the LED to be controlled.
 */
fn cli_led(
    slices: FixedStringSlices,
    led: &mut PB3<Output<PushPull>>,
) {
    if let Some(status) = slices.get(1) {
        print!("CLI: led {}\r\n", status.as_str());
        if status.as_str() == "on" {
            led.set_high();
        } else {
            led.set_low();
        }
    }
}

/**
 * This function controls the stepper motor based on the command received.
 * The command is expected to be in the first slice of the `slices` vector.
 *
 * @param slices: A vector of `String<SIZE_RX_BUFFER>`, each representing a slice of the received command.
 */
fn cli_stepper(slices: FixedStringSlices, stepper: &mut Stepper) {
    if let Some(degrees) = slices.get(1) {
        let move_degrees = degrees.parse().unwrap_or(0f32);
        if move_degrees == 0f32 {
            print!("Stepper: error, zero degree\r\n");
            return;
        }

        let mut speed_percent = 100f32;
        if let Some(speed) = slices.get(2) {
            speed_percent = speed.parse().unwrap_or(0f32);
        }

        stepper.set_parameters(move_degrees, speed_percent);
    }
}

/**
 * This function controls the state of the ADC monitor based on the command received.
 *
 * @param slices: A vector of `String<SIZE_RX_BUFFER>`, each representing a slice of the received command.
 * @param switch: A mutable reference to the boolean that controls the state of the ADC monitor.
 */
fn cli_adc(
    slices: FixedStringSlices,
    switch: &mut bool,
    interval_ms: &mut u32,
) {
    if let Some(status) = slices.get(1) {
        print!("CLI: adc {}\r\n", status.as_str());
        if status.as_str() == "on" {
            const MIN_INTERVAL: u32 = 10u32;
            const MAX_INTERVAL: u32 = 1000u32;

            if let Some(interval) = slices.get(2) {
                let interval_temp = interval.parse().unwrap_or(500u32);
                if (interval_temp >= MIN_INTERVAL) && (interval_temp <= MAX_INTERVAL) {
                    *interval_ms = interval_temp;
                }
            }

            *switch = true;
            print!("ADC: monitor on, interval={}ms\r\n", interval_ms);
            print!("ADC: simply enter or type 'adc off' to stop monitoring\r\n");
        } else {
            *switch = false;
            print!("ADC: monitor off\r\n");
        }
    }
}

/**
 * This function iterates over the slices of the received command and prints each slice to the serial sender.
 *
 * @param slices: A vector of `String<SIZE_RX_BUFFER>`, each representing a slice of the received command.
 */
#[allow(dead_code)]
fn cli_peek_slices(slices: FixedStringSlices) {
    for slice in slices {
        let maybe_num: Result<i32, _> = slice.parse();
        match maybe_num {
            Ok(num) => {
                print!("arg(num): {}\r\n", num);
            }
            Err(_) => {
                print!("arg(str): {}\r\n", slice);
            }
        }
    }
}

/**
 * This function reads from the serial receiver and splits the received string into slices based on spaces.
 * Each slice is then converted into a `String<SIZE_RX_BUFFER>` and stored in a `Vec`.
 * The function returns `None` if no data is received from the serial receiver, or if the received data does not end with a newline character.
 * If the received data ends with a newline character, the function returns a `Vec` of `String<SIZE_RX_BUFFER>`, each representing a slice of the received data.
 *
 * @param serial_rx: A mutable reference to the serial receiver.
 * @param str_buffer: A mutable reference to a `String<SIZE_RX_BUFFER>` that is used as a buffer to store the received data.
 * @return: An `Option` that contains a `Vec` of `String<SIZE_RX_BUFFER>` if the received data ends with a newline character, or `None` otherwise.
 */
fn get_command_slices(
    serial_rx: &mut hal::serial::Rx<USART2>,
    str_buffer: &mut String<SIZE_RX_BUFFER>,
) -> Option<FixedStringSlices> {
    let received = serial_rx.read().ok();

    if let Some(ch) = received {
        if ch != b'\n' {
            // push byte into the string buffer
            str_buffer.push(ch as char).unwrap();
            None
        } else {
            // NOTE: This is in order to fix a problem with the last number to be treated as a string when parsed.
            str_buffer.pop();
            str_buffer.push(' ').unwrap();

            let mut slices: FixedStringSlices = Vec::new();
            let mut start = 0;

            for (index, char) in str_buffer.char_indices() {
                if char == ' ' {
                    if start != index {
                        let slice_str = &str_buffer[start..index];
                        let mut slice_string: String<SIZE_RX_BUFFER> = String::new();
                        slice_string.push_str(slice_str).unwrap();
                        let _ = slices.push(slice_string);
                    }
                    start = index + 1;
                }
            }

            if start < str_buffer.len() {
                let slice_str = &str_buffer[start..];
                let mut slice_string: String<SIZE_RX_BUFFER> = String::new();
                slice_string.push_str(slice_str).unwrap();
                let _ = slices.push(slice_string);
            }

            str_buffer.clear();
            Some(slices)
        }
    } else {
        None
    }
}

/**
 * Initialize the hardware.
 *
 * @return The LED, TX, RX, and timer.
 */
#[rustfmt::skip]
fn init_hardware() -> (
    MonoTimer,
    PB3<Output<PushPull>>,
    Tx<USART2>,
    hal::serial::Rx<USART2>,
    PA4<Output<PushPull>>,
    PA5<Output<PushPull>>,
    PA6<Output<PushPull>>,
    PA7<Output<PushPull>>,
    ADC,
    PA0<Analog>,
    PB0<Input<PullDown>>,
    ) {
    // Setup the clock and etc
    let mut p = hal::stm32::Peripherals::take().unwrap();
    let mut rcc = p.RCC.constrain();

    let mut flash = p.FLASH.constrain();
    let mut pwr = p.PWR.constrain(&mut rcc.apb1r1);

    let clocks = rcc
        .cfgr
        .sysclk(80.MHz())
        .pclk1(80.MHz())
        .pclk2(80.MHz())
        .freeze(&mut flash.acr, &mut pwr);

    // Setup GPIO pins
    let mut gpioa = p.GPIOA.split(&mut rcc.ahb2);
    let pa0 = gpioa.pa0.into_analog(&mut gpioa.moder, &mut gpioa.pupdr);
    let pa2 = gpioa.pa2.into_alternate(&mut gpioa.moder, &mut gpioa.otyper, &mut gpioa.afrl);
    let pa3 = gpioa.pa3.into_alternate(&mut gpioa.moder, &mut gpioa.otyper, &mut gpioa.afrl);
    let pa4 = gpioa.pa4.into_push_pull_output(&mut gpioa.moder, &mut gpioa.otyper);
    let pa5 = gpioa.pa5.into_push_pull_output(&mut gpioa.moder, &mut gpioa.otyper);
    let pa6 = gpioa.pa6.into_push_pull_output(&mut gpioa.moder, &mut gpioa.otyper);
    let pa7 = gpioa.pa7.into_push_pull_output(&mut gpioa.moder, &mut gpioa.otyper);

    let mut gpiob = p.GPIOB.split(&mut rcc.ahb2);
    let pb3 = gpiob.pb3.into_push_pull_output(&mut gpiob.moder, &mut gpiob.otyper);

    // External interrupts
    let mut pb0 = gpiob.pb0.into_pull_down_input(&mut gpiob.moder, &mut gpiob.pupdr);
    pb0.make_interrupt_source(&mut p.SYSCFG, &mut rcc.apb2);
    pb0.trigger_on_edge(&mut p.EXTI, Edge::Rising);
    pb0.enable_interrupt(&mut p.EXTI);

    unsafe { 
        cortex_m::peripheral::NVIC::unmask(Interrupt::EXTI0); 
        cortex_m::peripheral::NVIC::pend(Interrupt::EXTI0); 
    }

    // Setup the serial pins
    let serial = Serial::usart2(p.USART2, (pa2, pa3), 115_200.bps(), clocks, &mut rcc.apb1r1);
    let (tx, rx) = serial.split();

    // Setup the timer
    let mut cp = cortex_m::Peripherals::take().unwrap();
    cp.DCB.enable_trace();
    cp.DWT.enable_cycle_counter();
    let timer: MonoTimer = MonoTimer::new(cp.DWT, clocks);

    // Setup the ADC
    let mut delay = Delay::new(cp.SYST, clocks);
    let adc = ADC::new(
        p.ADC1,
        p.ADC_COMMON,
        &mut rcc.ahb2,
        &mut rcc.ccipr,
        &mut delay,
    );

    (timer, pb3, tx, rx, pa4, pa5, pa6, pa7, adc, pa0, pb0)
}

/**
 * Panic handler.
 */
#[panic_handler]
fn panic(_panic: &PanicInfo<'_>) -> ! {
    loop {}
}

/**
 * EXTI0 interrupt handler.
 */
#[interrupt] 
fn EXTI0() { 
    cortex_m::peripheral::NVIC::unpend(hal::pac::Interrupt::EXTI0);
    let dp = unsafe { Peripherals::steal() };
    dp.EXTI.pr1.write(|w| w.pr0().set_bit());

    unsafe { 
        EXTI0_COUNT += 1; 
    }

    unsafe {
        *TEST.lock() = 10;        
    }
}