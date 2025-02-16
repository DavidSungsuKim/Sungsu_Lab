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
use embassy_time::Timer;
use embassy_sync::mutex::Mutex;
use heapless::{String, Vec};
use {defmt_rtt as _, panic_probe as _};
use nucleo_l432kc_embassy::stepper::{Stepper};

// Constants
const MAX_CLI_ARGS: usize = 20;
const SIZE_CLI_RX_BUFFER: usize = 64;

// Aliases
type FixedStringSlices = Vec<String<SIZE_CLI_RX_BUFFER>, MAX_CLI_ARGS>;
type StringCLI = String<SIZE_CLI_RX_BUFFER>;

// Global variables for communication between tasks
static LED_PERIOD_MS: Mutex<embassy_sync::blocking_mutex::raw::ThreadModeRawMutex, u64> = Mutex::new(1000);
static STEPPER_PARAMS: Mutex<embassy_sync::blocking_mutex::raw::ThreadModeRawMutex, (f32, f32)> = Mutex::new((0f32, 0f32));

/**
 * @brief LED task
 * @param led: the LED output
 */
#[embassy_executor::task]
async fn led_task(mut led: Output<'static>) {
    loop {
        let period_ms = LED_PERIOD_MS.lock().await.clone();

        led.set_high();
        Timer::after_millis(period_ms).await;
        led.set_low();
        Timer::after_millis(period_ms).await;
    }
}

/**
 * @brief Stepper task
 * @param stepper: the stepper motor
 */
#[embassy_executor::task]
async fn stepper_task(mut stepper: Stepper<'static>) {
    loop {
        let (degree, speed_percent) = STEPPER_PARAMS.lock().await.clone();
        if degree > 0f32 {
            stepper.set_parameters(degree, speed_percent);
            stepper.move_relative().await;
            *STEPPER_PARAMS.lock().await = (0f32, 0f32);
        }

        // to yield the CPU
        Timer::after_millis(100).await;
    }
}

/**
 * @brief CLI task
 * @param rx: the UART receiver
 */
#[embassy_executor::task]
async fn cli_task(mut rx: UartRx<'static, Async>) {
    let mut cmd_buffer: StringCLI = String::new();
    cmd_buffer.clear();

    loop {
        // wait a character
        let mut buffer = [0u8; 1];
        let _ = rx.read(&mut buffer).await;

        // parse the command
        if let Some(slices) = parse_cli_command(buffer[0], &mut cmd_buffer) {            
            let command = slices.get(0).unwrap();
            match command.as_str() {
                "led"  => { 
                    if let Some(period) = slices.get(1).and_then(|s| s.parse::<u64>().ok()) {
                        *LED_PERIOD_MS.lock().await = period;
                    }
                }
                "stepper" => {
                    if let Some(steps) = slices.get(1).and_then(|s| s.parse::<f32>().ok()) {
                        let speed_percent = slices.get(2).and_then(|s| s.parse::<f32>().ok()).unwrap_or(100f32);
                        *STEPPER_PARAMS.lock().await = (steps, speed_percent);
                    }
                }
                _ => { debug!("Undefined command: {}", command); }
            }
        }
    }
}

/**
 * @brief Main function
 * @param spawner: the spawner
 */
#[embassy_executor::main]
async fn main(spawner: Spawner) {
    let p = embassy_stm32::init(Default::default());

    let led = Output::new(p.PB3, Level::High, Speed::Low);

    let config = Config::default();
    let serial = Uart::new(p.USART2, p.PA3, p.PA2, Irqs, p.DMA1_CH7, p.DMA1_CH6, config).unwrap();
    let (serial_tx, serial_rx) = serial.split();

    let a_pos = Output::new(p.PA4, Level::Low, Speed::Low);
    let a_neg = Output::new(p.PA5, Level::Low, Speed::Low);
    let b_pos = Output::new(p.PA6, Level::Low, Speed::Low);
    let b_neg = Output::new(p.PA7, Level::Low, Speed::Low);
    let stepper = Stepper::new(a_pos, a_neg, b_pos, b_neg);

    /*
    const SIZE_BUFFER: usize = 128;
    let mut str_buffer: String<SIZE_BUFFER> = String::new();
    core::write!(&mut str_buffer, "Serial DMA\r\n").unwrap();
    let serial_fut = serial_tx.write(str_buffer.as_bytes());
    let _ = serial_fut.await;
    */

    /*
    let delay_fut = Timer::after_millis(100);
    let _ = delay_fut.await;
    */

    // Spawn the LED task
    let _ = spawner.spawn(led_task(led)).unwrap();
    let _ = spawner.spawn(cli_task(serial_rx)).unwrap();
    let _ = spawner.spawn(stepper_task(stepper)).unwrap();

    loop {
        // Yield the CPU to the executor by waiting for a timer to expire; 
        // this loop is supposed be sleeping most of the time.
        Timer::after_millis(1000).await;
    }
}

/**
 * @brief Parse the CLI command
 * @details This function gets a character and pushed it into a buffer until a carriage return is received.
 *          When a carriage return is received, the buffer is parsed into a vector of fixed string slices to form the command.
 * 
 * @param ch: the character to be parsed
 * @param buffer: the buffer to store the parsed command
 */
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

/**
 * @brief Bind the USART2 interrupts
 */
bind_interrupts!(struct Irqs {
    USART2 => usart::InterruptHandler<peripherals::USART2>;
});