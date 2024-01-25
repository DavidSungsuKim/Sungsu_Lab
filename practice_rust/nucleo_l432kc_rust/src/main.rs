#![no_main]
#![no_std]

extern crate cortex_m;
extern crate cortex_m_rt as runtime;
extern crate stm32l4xx_hal;

#[macro_use(block)]
extern crate nb;

use core::panic::PanicInfo;
use cortex_m::asm;
use stm32l4xx_hal as hal;
use crate::hal::prelude::*;
use crate::hal::serial::Serial;
use cortex_m_rt::entry;
use crate::hal::serial;
use crate::hal::pac::USART2;
use crate::serial::Tx;

use heapless::String;
use core::fmt;
use crate::hal::time;
use hal::time::MonoTimer;

#[entry]
fn main() -> ! {
    // common setup for the H/W
    let p = hal::stm32::Peripherals::take().unwrap(); 
    let mut rcc = p.RCC.constrain();

    // setup for the led toggling
    let mut gpiob = p.GPIOB.split( &mut rcc.ahb2 );
    let mut led = gpiob.pb3.into_push_pull_output( &mut gpiob.moder, &mut gpiob.otyper );

    // setup for the serial
    let mut flash = p.FLASH.constrain();
    let mut pwr = p.PWR.constrain(&mut rcc.apb1r1);
    let mut gpioa = p.GPIOA.split( &mut rcc.ahb2 );

    let clocks = rcc
    .cfgr
    .sysclk( 80.MHz() )
    .pclk1( 80.MHz() )
    .pclk2( 80.MHz() )
    .freeze( &mut flash.acr, &mut pwr );

    let tx = gpioa
    .pa2
    .into_alternate( &mut gpioa.moder, &mut gpioa.otyper, &mut gpioa.afrl );

    let rx = gpioa
    .pa3
    .into_alternate( &mut gpioa.moder, &mut gpioa.otyper, &mut gpioa.afrl );

    let serial = Serial::usart2( p.USART2, (tx, rx), 115_200.bps(), clocks, &mut rcc.apb1r1 );
    let ( mut tx, mut _rx ) = serial.split();

    // setup for the monotonic timer (under construction now...)
    //let mut mono_timer = MonoTimer::new( &mut p, clocks );

    // variables
    let mut str_rx_buffer: String<32> = String::new();
    str_rx_buffer.clear();

    let mut num = 1;
    let mut tick = 0;

    loop {

        // for led test
        tick += 1;
        if tick > 100000 {
            led.toggle();
            tick = 0;
        }

        // for serial test
        let received = _rx.read().ok();
        if let Some(ch) = received {

            // push byte into the string buffer
            str_rx_buffer.push( ch as char ).unwrap();
            if ch == b'\r' {
                // print out what's been received if it receives CR
                str_rx_buffer.push( b'\n' as char ).unwrap();
                send_bytes( &mut tx, &str_rx_buffer );
                str_rx_buffer.clear(); 
            }

            // let mut my_str: String<10> = String::new();
            // fmt::write( &mut my_str, format_args!( "rx: {}\r\n", ch as char) ).expect("err");
            // send_bytes( &mut tx, &my_str );
        }

        // let mut my_str2: String<20> = String::new();
        // fmt::write( &mut my_str2, format_args!( "tx: {}\r\n", num ) ).expect("err");
        // send_bytes( &mut tx, &my_str2 );
        // num += 1;
    }
}

fn wait_tick(count: u32) {
    for _i in 0..count {
        asm::nop()
    }
}

trait SendByte {
    fn send_byte(&mut self, byte: u8);
    fn send_bytes(&mut self, bytes: &str);    
}

impl SendByte for Tx<USART2> {
    fn send_byte(&mut self, byte: u8) {
        // here we have concrete Self: Tx<USART1>
        // so we can do whatever the type supports
        block!(self.write(byte)).ok();
    }   

    fn send_bytes(&mut self, bytes: &str) {
        for byte in bytes.bytes() {
            block!(self.write(byte)).ok();
        }       
    }      
}

fn send_byte<Tx: SendByte>(tx: &mut Tx, byte: u8) {
    tx.send_byte(byte);
}

fn send_bytes<Tx: SendByte>(tx: &mut Tx, bytes: &str) {
    tx.send_bytes(bytes);
}

#[panic_handler]
fn panic(_panic: &PanicInfo<'_>) -> ! {
    loop {}
}
