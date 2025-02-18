use crate::print;
use core::fmt::Write;
use embassy_stm32::mode::Async;
use embassy_stm32::usart::{UartTx};
use heapless::{String, Vec};
use spin::Mutex;

// Constants
const MAX_CLI_ARGS: usize = 20;
const SIZE_CLI_RX_BUFFER: usize = 64;

// Aliases
pub type FixedStringSlices = Vec<String<SIZE_CLI_RX_BUFFER>, MAX_CLI_ARGS>;
pub type StringCLI = String<SIZE_CLI_RX_BUFFER>;

// Global variables
pub static SERIAL_TX: Mutex<Option<UartTx<'_, Async>>> = Mutex::new(None);

/**
 * @brief Get the arguments for a CLI command.
 * @details This function gets a character and pushed it into a buffer until a carriage return is received.
 *          When a carriage return is received, the buffer is parsed into a vector of fixed string slices to form the command.
 * 
 * @param ch: the character to be parsed
 * @param buffer: the buffer to store the parsed command
 */
pub fn get_args(ch: u8, buffer: &mut StringCLI) -> Option<FixedStringSlices> {
   if ch != b'\r' {
       // push byte into the string buffer
       buffer.push(ch as char).unwrap();
       None
   } else {
       buffer.push(' ').unwrap(); // to fix a problem with the last number to be treated as a string when parsed.
   
       let mut args: FixedStringSlices = Vec::new();
       let mut start = 0;

       for (index, char) in buffer.char_indices() {
           if char == ' ' {
               if start != index {
                   let slice_str = &buffer[start..index];
                   let mut slice_string: StringCLI = String::new();
                   slice_string.push_str(slice_str).unwrap();
                   let _ = args.push(slice_string);
               }
               start = index + 1;
           }
       }

       if start < buffer.len() {
           let slice_str = &buffer[start..];
           let mut slice_string: StringCLI = String::new();
           slice_string.push_str(slice_str).unwrap();
           let _ = args.push(slice_string);
       }

       buffer.clear();
       Some(args)
   }
}

/**
 * @brief Create a singleton sender for the CLI.
 * @param serial_tx: the UART transmitter
 */
pub fn create_singleton_sender(serial_tx: UartTx<'static, Async>) {
   SERIAL_TX.lock().replace(serial_tx);
}

/**
 * @brief Clear the screen.
 */
pub async fn clear_screen() {
   print!("\x1B[2J\x1B[1;1H");
}

/**
 * @brief Print a welcome message to the serial port.
 */
pub async fn print_welcome() {
   print!("\r\n");
   print!(" ************************************\r\n");
   print!(" * Welcome to STM32L431 Rust Project - II\r\n");
   print!(" * Version: 1.0.0\r\n");
   print!(" * Author: SSKIM \r\n");
   print!(" ************************************\r\n");
}

/**
 * @brief Print a formatted string to the serial port.
 */
#[macro_export]
macro_rules! print {
      ($($arg:tt)*) => {{
            let mut guard = SERIAL_TX.lock();
            if let Some(serial_tx) = guard.as_mut() {
               const SIZE_BUFFER: usize = 128;
               let mut buffer = heapless::String::<SIZE_BUFFER>::new(); // Adjust buffer size as needed
               core::write!(buffer, $($arg)*).unwrap();
               let _ = serial_tx.write(buffer.as_bytes()).await;
            }
      }};
   }
