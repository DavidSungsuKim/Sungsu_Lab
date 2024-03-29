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

#[entry]
fn main() -> ! {
    let (mut led, tx, mut _rx, timer) = init_hardware();

    let mut formatter = SerialFormatter::new(tx);

    // other variables
    let mut str_rx_buffer: String<32> = String::new();
    str_rx_buffer.clear();

    let get_tick_ms = |ms: u32| -> u32 { 
        timer.frequency().to_Hz() / 1000 * ms
    };

    let tick_cnt_for_action = get_tick_ms(1000);
    let mut time_tick = timer.now();

    send!(formatter, "Welcome to STM32L431 Rust Project\r\n");
    
    // main loop
    loop {
        if time_tick.elapsed() > tick_cnt_for_action {
            time_tick = timer.now();

            // toggle led
            led.toggle();
        }

        // for serial test
        let received = _rx.read().ok();
        if let Some(ch) = received {
            if ch != b'\n' {
                // push byte into the string buffer
                str_rx_buffer.push( ch as char ).unwrap();
            }
            else {

                let( command, option ) = parse_command( &str_rx_buffer );
                
                // echo what it has received
                send!(formatter, "Command:{} option:{}\r\n", command, option.unwrap_or("")); 
            }
        }
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

fn parse_command(input: &str) -> (&str, Option<&str>) {
    let mut parts: Vec<&str, 2> = Vec::new();
    
    let mut start = 0;
    
    for (index, character) in input.char_indices() {
        if character == ' ' {
            if start != index {
                parts.push(&input[start..index]).ok();
            }
            start = index + 1;
        }
    }
    
    if start < input.len() {
        parts.push(&input[start..]).ok();
    }
    
    match parts.as_slice() {
        [command, option] => (*command, Some(*option)),
        [command] => (*command, None),
        _ => ("", None),
    }
}

trait SendByte {
    fn send_byte(&mut self, byte: u8);
    fn send_bytes(&mut self, bytes: &str);    
}

impl SendByte for Tx<USART2> {
    fn send_byte(&mut self, byte: u8) {
        block!(self.write(byte)).ok();
    }   

    fn send_bytes(&mut self, bytes: &str) {
        for byte in bytes.bytes() {
            self.send_byte(byte);
        }       
    }      
}

struct SerialFormatter<T: SendByte> {
    tx: T,
}

impl<T: SendByte> SerialFormatter<T> {
    fn new(tx: T) -> Self {
        SerialFormatter { tx }
    }

    fn send_formatted(&mut self, format: core::fmt::Arguments) {
        let mut buffer = heapless::String::<64>::new(); // Adjust buffer size as needed
        write!(buffer, "{}", format).unwrap();
        self.tx.send_bytes(&buffer);
    }
}

// Macro to simplify sending formatted strings
#[macro_export]
macro_rules! send {
    ($formatter:expr, $($arg:tt)*) => {{
        $formatter.send_formatted(format_args!($($arg)*));
    }};
}

#[panic_handler]
fn panic(_panic: &PanicInfo<'_>) -> ! {
    loop {}
}
