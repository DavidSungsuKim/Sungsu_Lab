#![no_main]
#![no_std]

extern crate cortex_m;
extern crate cortex_m_rt as runtime;
extern crate stm32f4xx_hal;

use core::panic::PanicInfo;
use cortex_m::asm;
use stm32f4xx_hal as hal;
use crate::hal::{pac, prelude::*};
use cortex_m_rt::entry;

#[entry]
fn main() -> ! {
    let p = pac::Peripherals::take().unwrap();

    let gpioa = p.GPIOA.split();
    let mut led = gpioa.pa5.into_push_pull_output();

    loop {
        wait_tick( 100000 );
        led.set_high();
        
        wait_tick( 1000 ); 
        led.set_low();
    }
}

fn wait_tick( count : u32 ) {
    for _i in 0..count {
        asm::nop()
    }
}

#[panic_handler]
fn panic(_panic: &PanicInfo<'_>) -> ! {
    loop {}
}