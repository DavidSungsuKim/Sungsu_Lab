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
use hal::gpio::Output;
use hal::gpio::PushPull;
use heapless::String;
use heapless::Vec;
use core::fmt::Write;

const SIZE_TX_BUFFER: usize = 128;
const SIZE_RX_BUFFER: usize = 64;
const MAX_ARGS: usize = 20;

#[entry]
/**
 * Main function.
 */
fn main() -> ! {
    let (mut led, serial_tx, mut serial_rx, timer) = init_hardware();

    let mut sender = SerialSender::new(serial_tx);

    let mut str_buffer: String<SIZE_RX_BUFFER> = String::new();
    str_buffer.clear();

    let get_tick_ms = |ms: u32| -> u32 { 
        timer.frequency().to_Hz() / 1000 * ms
    };

    let tick_cnt_for_action = get_tick_ms(1000);
    let mut time_tick = timer.now();

    print!(sender, "************************************\r\n");
    print!(sender, "* Welcome to STM32L431 Rust Project\r\n");
    print!(sender, "* Version: 0.0.0\r\n");
    print!(sender, "* Author: sskim \r\n");
    print!(sender, "************************************\r\n");

    print!(sender, "Enter a command...\r\n");

    loop {
        if time_tick.elapsed() > tick_cnt_for_action {
            time_tick = timer.now();

            // toggle led
            led.toggle();
        }

        if let Some(slices) = get_command_slices(&mut serial_rx, &mut str_buffer) {
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
    }
}

fn get_command_slices(serial_rx: &mut hal::serial::Rx<USART2>, str_buffer: &mut String<SIZE_RX_BUFFER>) -> Option<Vec<String<SIZE_RX_BUFFER>, MAX_ARGS>> {
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

            let mut slices: Vec<String<SIZE_RX_BUFFER>, MAX_ARGS> = Vec::new();
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
fn init_hardware() -> (PB3<Output<PushPull>>, Tx<USART2>, hal::serial::Rx<USART2>, MonoTimer) {
    let p = hal::stm32::Peripherals::take().unwrap();
    let mut rcc = p.RCC.constrain();

    let mut gpiob = p.GPIOB.split(&mut rcc.ahb2);
    let led = gpiob.pb3.into_push_pull_output(&mut gpiob.moder, &mut gpiob.otyper);

    let mut flash = p.FLASH.constrain();
    let mut pwr = p.PWR.constrain(&mut rcc.apb1r1);
    let mut gpioa = p.GPIOA.split(&mut rcc.ahb2);

    let clocks = rcc.cfgr.sysclk(80.MHz()).pclk1(80.MHz()).pclk2(80.MHz()).freeze(&mut flash.acr, &mut pwr);

    let tx = gpioa.pa2.into_alternate(&mut gpioa.moder, &mut gpioa.otyper, &mut gpioa.afrl);
    let rx = gpioa.pa3.into_alternate(&mut gpioa.moder, &mut gpioa.otyper, &mut gpioa.afrl);

    let serial = Serial::usart2(p.USART2, (tx, rx), 115_200.bps(), clocks, &mut rcc.apb1r1);
    let (tx, rx) = serial.split();

    let mut cp = cortex_m::Peripherals::take().unwrap();
    cp.DCB.enable_trace();
    cp.DWT.enable_cycle_counter();
    let timer: MonoTimer = MonoTimer::new(cp.DWT, clocks);

    (led, tx, rx, timer)
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
