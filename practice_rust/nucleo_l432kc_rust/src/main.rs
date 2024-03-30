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
fn main() -> ! {
    let (mut led, serial_tx, mut serial_rx, timer) = init_hardware();

    let mut sender = SerialSender::new(serial_tx);

    let mut str_rx_buffer: String<SIZE_RX_BUFFER> = String::new();
    str_rx_buffer.clear();

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

    loop {
        if time_tick.elapsed() > tick_cnt_for_action {
            time_tick = timer.now();

            // toggle led
            led.toggle();
        }

        // for serial test
        let received = serial_rx.read().ok();
        if let Some(ch) = received {
            if ch != b'\n' {
                // push byte into the string buffer
                str_rx_buffer.push( ch as char ).unwrap();
            }
            else {
                let args = split_string( &mut str_rx_buffer );
                for arg in args {
                    print!(sender, "arg: {}\r\n", arg);
                }
                str_rx_buffer.clear();
            }
        }
    }
}

fn split_string(input: &mut str) -> Vec<&str, MAX_ARGS> {
    let mut parts: Vec<&str, MAX_ARGS> = Vec::new();
    let mut start = 0;

    for (index, character) in input.char_indices() {
        if character == ' ' {
            if start != index {
                let _ = parts.push(&input[start..index]);
            }
            start = index + 1;
        }
    }

    if start < input.len() {
        let _ = parts.push(&input[start..]);
    }

    parts
}

trait SendByte {
    fn send_bytes(&mut self, bytes: &str);  
    fn send_byte(&mut self, byte: u8);
}

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

struct SerialSender<T: SendByte> {
    tx: T,
}

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

// Macro to simplify sending formatted strings
#[macro_export]
macro_rules! print {
    ($sender:expr, $($arg:tt)*) => {{
        $sender.send_formatted(format_args!($($arg)*));
    }};
}

#[panic_handler]
fn panic(_panic: &PanicInfo<'_>) -> ! {
    loop {}
}
