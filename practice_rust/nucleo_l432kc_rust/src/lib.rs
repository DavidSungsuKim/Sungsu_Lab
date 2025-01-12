
#![no_std]

extern crate cortex_m;
extern crate cortex_m_rt as runtime;
extern crate stm32l4xx_hal;

#[macro_use(block)]
extern crate nb;

pub mod serial_sender;