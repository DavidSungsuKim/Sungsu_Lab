#![no_std]
#![no_main]

use defmt::debug;
use core::fmt::Write;
use embassy_executor::Spawner;
use embassy_futures::join::join;
use embassy_stm32::gpio::{Level, Output, Speed};
use embassy_stm32::usart::{Config, Uart, UartRx};
use embassy_stm32::mode::Async;
use embassy_stm32::{bind_interrupts, peripherals, usart};
use embassy_time::{Timer, Instant};
use heapless::{String, Vec};
use {defmt_rtt as _, panic_probe as _};

// constants
const SIZE_BUFFER: usize = 128;
const MAX_CLI_ARGS: usize = 20;
const SIZE_CLI_RX_BUFFER: usize = 64;

// aliases
type FixedStringSlices = Vec<String<SIZE_CLI_RX_BUFFER>, MAX_CLI_ARGS>;
type StringCLI = String<SIZE_CLI_RX_BUFFER>;

bind_interrupts!(struct Irqs {
    USART2 => usart::InterruptHandler<peripherals::USART2>;
});

#[embassy_executor::task]
async fn led_task(mut led: Output<'static>) {
    loop {
        led.set_high();
        Timer::after_millis(1000).await;
        led.set_low();
        Timer::after_millis(1000).await;
    }
}

#[embassy_executor::task]
async fn cli_task(mut rx: UartRx<'static, Async>) {

    let mut cli_buffer: StringCLI = String::new();
    cli_buffer.clear();

    loop {
        // wait a character
        let mut buffer = [0u8; 1];
        let _ = rx.read(&mut buffer).await;

        // parse the command
        if let Some(slices) = parse_cli_command(buffer[0], &mut cli_buffer) {            
            for slice in slices.iter() {
                debug!("slice: {:?}", slice);
            }
        }
    }
}

#[embassy_executor::main]
async fn main(spawner: Spawner) {
    let p = embassy_stm32::init(Default::default());

    let led = Output::new(p.PB3, Level::High, Speed::Low);

    let config = Config::default();
    let serial = Uart::new(p.USART2, p.PA3, p.PA2, Irqs, p.DMA1_CH7, p.DMA1_CH6, config).unwrap();

    let (mut serial_tx, mut serial_rx) = serial.split();
    let mut str_buffer: String<SIZE_BUFFER> = String::new();

    // Spawn the LED task
    let _ = spawner.spawn(led_task(led)).unwrap();
    let _ = spawner.spawn(cli_task(serial_rx)).unwrap();

    let mut i = 0;
    loop {    
        /*
        let delay_fut = Timer::after_millis(100);

        core::write!(&mut str_buffer, "Serial DMA{}!\r\n", i).unwrap();
        let serial_fut = serial_tx.write(str_buffer.as_bytes());

        // Wait for both futures to complete
        let start = Instant::now();

        let _ = join(delay_fut, serial_fut).await;
        str_buffer.clear();
      
        let elapsed = start.elapsed();
        */
        //debug!("Elapsed: {:?}", elapsed.as_millis());

        //debug!("time2");
        Timer::after_millis(1000).await;
        //debug!("time2 wait done");

        i += 1;
    }
}

fn parse_cli_command(ch: u8, buffer: &mut StringCLI) -> Option<FixedStringSlices> {
    if ch != b'\r' {
        // push byte into the string buffer
        buffer.push(ch as char).unwrap();
        None
    } else {
        buffer.push(' ').unwrap(); // to fix a problem with the last number to be treated as a string when parsed.
    
        let mut slices: FixedStringSlices = Vec::new();
        let mut start = 0;

        for (index, char) in buffer.char_indices() {
            if char == ' ' {
                if start != index {
                    let slice_str = &buffer[start..index];
                    let mut slice_string: StringCLI = String::new();
                    slice_string.push_str(slice_str).unwrap();
                    let _ = slices.push(slice_string);
                }
                start = index + 1;
            }
        }

        if start < buffer.len() {
            let slice_str = &buffer[start..];
            let mut slice_string: StringCLI = String::new();
            slice_string.push_str(slice_str).unwrap();
            let _ = slices.push(slice_string);
        }

        buffer.clear();
        Some(slices)
    }
}