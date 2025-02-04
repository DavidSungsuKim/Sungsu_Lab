#![no_std]
#![no_main]

use defmt::debug;
use core::fmt::Write;
use embassy_executor::Spawner;
use embassy_futures::join::join;
use embassy_stm32::gpio::{Level, Output, Speed};
use embassy_stm32::usart::{Config, Uart};
use embassy_stm32::{bind_interrupts, peripherals, usart};
use embassy_time::{Timer, Instant};
use heapless::String;
use {defmt_rtt as _, panic_probe as _};

const SIZE_BUFFER: usize = 128;

bind_interrupts!(struct Irqs {
    USART2 => usart::InterruptHandler<peripherals::USART2>;
});

#[embassy_executor::main]
async fn main(_spawner: Spawner) {
    let p = embassy_stm32::init(Default::default());

    let mut led = Output::new(p.PB3, Level::High, Speed::Low);

    let config = Config::default();
    let mut serial = Uart::new(p.USART2, p.PA3, p.PA2, Irqs, p.DMA1_CH7, p.DMA1_CH6, config).unwrap();
    let mut str_buffer: String<SIZE_BUFFER> = String::new();

    // Clear the screen
    core::write!(&mut str_buffer, "\x1b[H\x1b[2J").unwrap();
    serial.write(str_buffer.as_bytes()).await.ok();
    str_buffer.clear();

    let mut i = 0;
    loop {    
        let delay_fut = Timer::after_millis(100);

        core::write!(&mut str_buffer, "Serial DMA{}!\r\n", i).unwrap();
        let serial_fut = serial.write(str_buffer.as_bytes());

        // Wait for both futures to complete
        let start = Instant::now();

        led.set_high();
        let _ = join(delay_fut, serial_fut).await;
        str_buffer.clear();
      
        let elapsed = start.elapsed();
        debug!("Elapsed: {:?}", elapsed.as_millis());

        led.set_low();
        debug!("time2");
        Timer::after_millis(1000).await;
        debug!("time2 wait done");

        i += 1;
    }
}
