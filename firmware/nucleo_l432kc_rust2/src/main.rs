#![no_std]
#![no_main]

use core::fmt::Write;
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

    let mut led = Output::new(p.PB3, Level::High, Speed::Low);

    let config = Config::default();
    let mut serial = Uart::new(p.USART2, p.PA3, p.PA2, Irqs, p.DMA1_CH7, p.DMA1_CH6, config).unwrap();
    let mut s: String<128> = String::new();

    // Clear the screen
    core::write!(&mut s, "\x1b[H\x1b[2J").unwrap();
    serial.write(s.as_bytes()).await.ok();
    s.clear();

    let mut i = 0;
    loop {        
        // Write to the serial. Transmission starts immediately.
        core::write!(&mut s, "Hello DMA World {}!\r\n", i).unwrap();
        let future = serial.write(s.as_bytes());
    
        // Turn on the LED 
        led.set_high();

        // Wait for the serial to finish writing
        future.await.ok();
        s.clear();

        // Wait for 1 seconds
        Timer::after_millis(1000).await;   

        // Turn off the LED
        led.set_low();

        i += 1;
    }
}
