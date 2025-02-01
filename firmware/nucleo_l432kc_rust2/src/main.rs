#![no_std]
#![no_main]

use defmt::debug;
use core::fmt::Write;
use embassy_executor::Spawner;
use embassy_stm32::gpio::{Level, Output, Speed};
use embassy_stm32::usart::{Config, Uart};
use embassy_stm32::{bind_interrupts, peripherals, usart};
use embassy_time::Timer;
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

        led.set_high();
        debug!("time");
        let future_time = Timer::after_millis(1000);

        debug!("serial");
        core::write!(&mut str_buffer, "Serial DMA{}!\r\n", i).unwrap();
        let future_serial = serial.write(str_buffer.as_bytes());

        future_serial.await.ok();
        debug!("serial done");

        future_time.await;
        debug!("time wait done");

        str_buffer.clear();
      
        led.set_low();
        debug!("time2");
        Timer::after_millis(1000).await;
        debug!("time2 wait done");

        i += 1;
    }
}
