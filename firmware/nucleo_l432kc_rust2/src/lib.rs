
#![no_std]

pub mod stepper;

#[macro_use]
pub mod cli;

// Aliases
pub type MutexEmbassy = embassy_sync::blocking_mutex::raw::ThreadModeRawMutex;