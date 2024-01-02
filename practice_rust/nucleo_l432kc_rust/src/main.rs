#![no_main]
#![no_std]

extern crate cortex_m;
extern crate cortex_m_rt as runtime;
extern crate stm32l4xx_hal;

use core::panic::PanicInfo;
use cortex_m::asm;
use stm32l4xx_hal as hal;
use crate::hal::prelude::*;
use cortex_m_rt::entry;

#[entry]
fn main() -> ! {
    let p = hal::stm32::Peripherals::take().unwrap(); 

    let mut rcc = p.RCC.constrain(); 
    let mut gpiob: hal::gpio::gpiob::Parts = p.GPIOB.split( &mut rcc.ahb2 );
    let mut led= gpiob.pb3.into_push_pull_output( &mut gpiob.moder, &mut gpiob.otyper );

    loop {
        wait_tick( 10000 );
        led.set_high();
        
        wait_tick( 10000 ); 
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