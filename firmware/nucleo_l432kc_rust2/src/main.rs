#![no_std]
#![no_main]

use core::fmt::Write;
use defmt::*;
use embassy_executor::Spawner;
use embassy_stm32::gpio::{Level, Output, Speed};
use embassy_stm32::usart::{Config, Uart};
use embassy_stm32::{bind_interrupts, peripherals, usart};
use embassy_time::Timer;
use heapless::String;
use {defmt_rtt as _, panic_probe as _};

bind_interrupts!(struct Irqs {
    USART2 => usart::InterruptHandler<peripherals::USART2>;
});

#[embassy_executor::main]
async fn main(_spawner: Spawner) {
    let p = embassy_stm32::init(Default::default());
    info!("Hello World!");

    let mut led = Output::new(p.PB3, Level::High, Speed::Low);

    let config = Config::default();
    let mut usart = Uart::new(p.USART2, p.PA3, p.PA2, Irqs, p.DMA1_CH7, p.DMA1_CH6, config).unwrap();

    let mut i = 0;
    loop {
        led.set_high();
        Timer::after_millis(30).await;        
        led.set_low();
        let future = Timer::after_millis(1000);
        future.await;

        println!("{}", i);
        i += 1;

        // for n in 0u32.. {
        //     let mut s: String<128> = String::new();
        //     core::write!(&mut s, "Hello DMA World {}!\r\n", n).unwrap();
        //     //info!("Writing...");
        //     usart.write(s.as_bytes()).await.ok();
        // }

        let mut s: String<128> = String::new();
        core::write!(&mut s, "Hello DMA World {}!\r\n", i).unwrap();
        usart.write(s.as_bytes()).await.ok();
    }
}
