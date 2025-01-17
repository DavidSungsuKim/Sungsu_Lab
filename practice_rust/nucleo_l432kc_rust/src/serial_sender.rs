/*
 * @file serial_sender.rs
 * @brief Rust library for stepper motor control using STM32L4 microcontroller
 * @details This file contains the implementation of the SerialSender struct and the SendByte trait.
 *          The SendByte trait is implemented for the Tx<USART2> struct to send bytes.
 *          The SerialSender struct is used to send formatted strings over USART2.
 *
 * @author Sungsu Kim
 * @date 2025-01-11
 * @version 1.1.0
 */

use core::fmt::Write;
use hal::pac::USART2;
use hal::prelude::*;
use hal::serial::Tx;
use stm32l4xx_hal as hal;

const SIZE_TX_BUFFER: usize = 128;

/**
 * Trait for sending bytes.
 */
pub trait SendByte {
    fn send_bytes(&mut self, bytes: &str);
    fn send_byte(&mut self, byte: u8);
}

/**
 * Implementation of the SendByte trait for the USART2.
 */
impl SendByte for Tx<USART2> {
    fn send_bytes(&mut self, bytes: &str) {
        for byte in bytes.bytes() {
            self.send_byte(byte);
        }
    }

    fn send_byte(&mut self, byte: u8) {
        block!(self.write(byte)).ok();
    }
}

/**
 * Struct for sending formatted strings.
 */
pub struct SerialSender<T: SendByte> {
    tx: T,
}

/**
 * Implementation of the SerialSender trait.
 */
impl<T: SendByte> SerialSender<T> {
    pub fn new(tx: T) -> Self {
        SerialSender { tx }
    }

    pub fn send_formatted(&mut self, format: core::fmt::Arguments) {
        let mut buffer = heapless::String::<SIZE_TX_BUFFER>::new(); // Adjust buffer size as needed
        write!(buffer, "{}", format).unwrap();
        self.tx.send_bytes(&buffer);
        buffer.clear();
    }
}

/**
 * Macro to simplify sending formatted strings.
 */
#[macro_export]
macro_rules! print {
    ($sender:expr, $($arg:tt)*) => {{
        $sender.send_formatted(format_args!($($arg)*));
    }};
}
