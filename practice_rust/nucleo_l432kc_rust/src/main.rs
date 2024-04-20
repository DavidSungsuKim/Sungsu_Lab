#![no_main]
#![no_std]

extern crate cortex_m;
extern crate cortex_m_rt as runtime;
extern crate stm32l4xx_hal;

#[macro_use(block)]
extern crate nb;

use core::panic::PanicInfo;
use cortex_m_rt::entry;
use stm32l4xx_hal as hal;
use hal::prelude::*;
use hal::serial::{Serial, Tx};
use hal::pac::USART2;
use hal::time::MonoTimer;
use hal::gpio::gpiob::PB3;
use hal::gpio::gpioa::PA4;
use hal::gpio::gpioa::PA5;
use hal::gpio::gpioa::PA6;
use hal::gpio::gpioa::PA7;
use hal::gpio::Output;
use hal::gpio::PushPull;
use heapless::String;
use heapless::Vec;
use core::fmt::Write;

const SIZE_TX_BUFFER: usize = 128;
const SIZE_RX_BUFFER: usize = 64;
const MAX_ARGS: usize = 20;

type FixedStringSlices = Vec<String<SIZE_RX_BUFFER>, MAX_ARGS>;

#[entry]
/**
 * Main function.
 */
fn main() -> ! {
    let (mut led, serial_tx, mut serial_rx, timer, mut a_pos, mut a_neg, mut b_pos, mut b_neg) = init_hardware();

    let mut sender = SerialSender::new(serial_tx);

    let mut str_buffer: String<SIZE_RX_BUFFER> = String::new();
    str_buffer.clear();

    let get_tick_ms = |ms: u32| -> u32 { 
        timer.frequency().to_Hz() / 1000 * ms
    };

    let tick_cnt_for_action = get_tick_ms(1000);
    let mut time_tick = timer.now();
    let mut stepper_seq : u32 = 0;
    cli_print_info(&mut sender);
    print!(sender, "Enter a command...\r\n");

    loop {
        if time_tick.elapsed() > tick_cnt_for_action {
            time_tick = timer.now();

            // toggle led
            //led.toggle();
        }

        if let Some(slices) = get_command_slices(&mut serial_rx, &mut str_buffer) {
            print!(sender, "CLI: #args={}\r\n", slices.len());
    
            let command = slices.get(0).unwrap();
            match command.as_str() {
                "info" => {
                    cli_print_info(&mut sender);
                }
                "led" => {
                    cli_control_led(slices.clone(), &mut sender, &mut led);
                }
                "stepper" => {
                    cli_control_stepper(slices.clone(), &mut sender, &mut a_pos, &mut a_neg, &mut b_pos, &mut b_neg, &mut stepper_seq );
                }
                _ => {
                }
            }
        }
    }
}

/**
 * This function prints the welcome message and project information to the serial sender.
 *
 * @param sender: A mutable reference to the `SerialSender` used to send the information.
 */
fn cli_print_info(sender: &mut SerialSender<Tx<USART2>>)
{
    print!(sender, "************************************\r\n");
    print!(sender, "* Welcome to STM32L431 Rust Project\r\n");
    print!(sender, "* Version: 1.0.0\r\n");
    print!(sender, "* Author: sskim \r\n");
    print!(sender, "************************************\r\n");
}

/**
 * This function controls the state of an LED based on the command received.
 * The command is expected to be in the first slice of the `slices` vector.
 * If the command is "on", the LED is set to high. Otherwise, the LED is set to low.
 *
 * @param slices: A vector of `String<SIZE_RX_BUFFER>`, each representing a slice of the received command.
 * @param sender: A mutable reference to the `SerialSender` used to send responses.
 * @param led: A mutable reference to the LED to be controlled.
 */
fn cli_control_led(slices: FixedStringSlices, sender: &mut SerialSender<Tx<USART2>>, led: &mut PB3<Output<PushPull>>)
{
    if let Some(status) = slices.get(1) {
        print!(sender, "CLI: led {}\r\n", status.as_str());
        if status.as_str() == "on" {
            led.set_high();
        }
        else {
            led.set_low();
        }
    }
}

fn cli_control_stepper(slices: FixedStringSlices, sender: &mut SerialSender<Tx<USART2>>, 
                        a_pos: &mut PA4<Output<PushPull>>, a_neg: &mut PA5<Output<PushPull>>, b_pos: &mut PA6<Output<PushPull>>, b_neg: &mut PA7<Output<PushPull>>, stepper_seq: &mut u32)
{
    if let Some(dir) = slices.get(1) {

        print!(sender, "CLI: stepper {}\r\n", dir.as_str());

        match dir.as_str() {
            "+" => { 
                *stepper_seq += 1; 
                if *stepper_seq > 3 {
                    *stepper_seq = 0;
                }
            }
            "-" => { 
                if *stepper_seq == 0 {
                    *stepper_seq = 3;
                }
                else  {
                    *stepper_seq -= 1;
                }
            }
            _ => { }
        }

        match stepper_seq {
            0 => {
                a_pos.set_high();
                a_neg.set_low();
                b_pos.set_low();
                b_neg.set_low();
            }
            1 => {
                a_pos.set_low();
                a_neg.set_high();
                b_pos.set_low();
                b_neg.set_low();
            }
            2 => {
                a_pos.set_low();
                a_neg.set_low();
                b_pos.set_high();
                b_neg.set_low();
            }
            3 => {
                a_pos.set_low();
                a_neg.set_low();
                b_pos.set_low();
                b_neg.set_high();
            }
            _ => 
            {}
        }
    }
}

/**
 * This function iterates over the slices of the received command and prints each slice to the serial sender.
 *
 * @param slices: A vector of `String<SIZE_RX_BUFFER>`, each representing a slice of the received command.
 * @param sender: A mutable reference to the `SerialSender` used to send responses.
 */
/*
fn cli_peek_slices(slices: FixedStringSlices, sender: &mut SerialSender<Tx<USART2>>)
{
    for slice in slices {
        let maybe_num: Result<i32, _> = slice.parse();
        match maybe_num {
            Ok(num) => {
                print!(sender, "arg(num): {}\r\n", num);
            }
            Err(_) => {
                print!(sender, "arg(str): {}\r\n", slice);
            }
        }
    }
}
*/

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
fn get_command_slices(serial_rx: &mut hal::serial::Rx<USART2>, str_buffer: &mut String<SIZE_RX_BUFFER>) -> Option<FixedStringSlices> {
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
 * Trait for sending bytes.
 */
trait SendByte {
    fn send_bytes(&mut self, bytes: &str);  
    fn send_byte(&mut self, byte: u8);
}

/**
 * Implementation of the SendByte trait for the USART2.
 */
impl SendByte for Tx<USART2> {
    fn send_bytes(&mut self, bytes: &str) {
        for byte in bytes.bytes() {
            self.send_byte(byte);
        }       
    }   

    fn send_byte(&mut self, byte: u8) {
        block!(self.write(byte)).ok();
    }   
}

/**
 * Struct for sending formatted strings.
 */
struct SerialSender<T: SendByte> {
    tx: T,
}

/**
 * Implementation of the SerialSender trait.
 */
impl<T: SendByte> SerialSender<T> {
    fn new(tx: T) -> Self {
        SerialSender { tx }
    }

    fn send_formatted(&mut self, format: core::fmt::Arguments) {
        let mut buffer = heapless::String::<SIZE_TX_BUFFER>::new(); // Adjust buffer size as needed
        write!(buffer, "{}", format).unwrap();
        self.tx.send_bytes(&buffer);
        buffer.clear();
    }
}

/**
 * Initialize the hardware.
 * 
 * @return The LED, TX, RX, and timer.
 */
fn init_hardware() -> (PB3<Output<PushPull>>, Tx<USART2>, hal::serial::Rx<USART2>, MonoTimer, PA4<Output<PushPull>>, PA5<Output<PushPull>>, PA6<Output<PushPull>>, PA7<Output<PushPull>>) {

    // Setup the clock and etc
    let p = hal::stm32::Peripherals::take().unwrap();
    let mut rcc = p.RCC.constrain();

    let mut flash = p.FLASH.constrain();
    let mut pwr = p.PWR.constrain(&mut rcc.apb1r1);

    let clocks = rcc.cfgr.sysclk(80.MHz()).pclk1(80.MHz()).pclk2(80.MHz()).freeze(&mut flash.acr, &mut pwr);

    // Setup GPIO pins
    let mut gpioa = p.GPIOA.split(&mut rcc.ahb2);
    let tx = gpioa.pa2.into_alternate(&mut gpioa.moder, &mut gpioa.otyper, &mut gpioa.afrl);
    let rx = gpioa.pa3.into_alternate(&mut gpioa.moder, &mut gpioa.otyper, &mut gpioa.afrl);

    let pin_stepper_a_pos = gpioa.pa4.into_push_pull_output(&mut gpioa.moder, &mut gpioa.otyper);
    let pin_stepper_a_neg = gpioa.pa5.into_push_pull_output(&mut gpioa.moder, &mut gpioa.otyper);
    let pin_stepper_b_pos = gpioa.pa6.into_push_pull_output(&mut gpioa.moder, &mut gpioa.otyper);
    let pin_stepper_b_neg = gpioa.pa7.into_push_pull_output(&mut gpioa.moder, &mut gpioa.otyper);

    let mut gpiob = p.GPIOB.split(&mut rcc.ahb2);
    let led = gpiob.pb3.into_push_pull_output(&mut gpiob.moder, &mut gpiob.otyper);

    // Setup the serial pins
    let serial = Serial::usart2(p.USART2, (tx, rx), 115_200.bps(), clocks, &mut rcc.apb1r1);
    let (tx, rx) = serial.split();

    // Setup the timer
    let mut cp = cortex_m::Peripherals::take().unwrap();
    cp.DCB.enable_trace();
    cp.DWT.enable_cycle_counter();
    let timer: MonoTimer = MonoTimer::new(cp.DWT, clocks);

    (led, tx, rx, timer, pin_stepper_a_pos, pin_stepper_a_neg, pin_stepper_b_pos, pin_stepper_b_neg)
}

/**
 * Macro to simplify sending formatted strings.
 */
#[macro_export]
macro_rules! print {
    ($sender:expr, $($arg:tt)*) => {{
        $sender.send_formatted(format_args!($($arg)*));
    }};
}

/**
 * Panic handler.
 */
#[panic_handler]
fn panic(_panic: &PanicInfo<'_>) -> ! {
    loop {}
}
