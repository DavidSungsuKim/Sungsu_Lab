/*
 * @file lib.rs
 * @brief Rust library for stepper motor control using STM32L4 microcontroller
 *
 * @author Sungsu Kim
 * @date 2025-01-11
 * @version 1.1.0
 */

#![no_std]

extern crate cortex_m;
extern crate cortex_m_rt as runtime;
extern crate stm32l4xx_hal;

#[macro_use(block)]
extern crate nb;

#[macro_use]
pub mod serial_sender;

pub mod stepper;
