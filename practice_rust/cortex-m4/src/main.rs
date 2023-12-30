#![no_main]
#![no_std]

extern crate cortex_m;
extern crate cortex_m_rt as runtime;
extern crate stm32f4;

use core::panic::PanicInfo;
use stm32f4::stm32f401::Peripherals;
use cortex_m::asm;

#[no_mangle]
fn main() -> ! {
    let per = Peripherals::take().unwrap();

    // Enable the clock for GPIOA
    per.RCC.ahb1enr.write(|w| w.gpioaen().bit(true));
    
    // Configure pin as output
    per.GPIOA.moder.write(|w| w.moder5().bits(0b01));

    // can't return so we go into an infinite loop here
    loop {
        // Toggle the LED output
        per.GPIOA.odr.modify(|r,w| w.odr5().bit(r.odr5().bit_is_clear()));     

        for _i in 0..5000 {
             asm::nop()
        }
    }
}

#[panic_handler]
fn panic(_panic: &PanicInfo<'_>) -> ! {
    loop {}
}