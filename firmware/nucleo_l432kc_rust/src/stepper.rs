/*
 * @file stepper.rs
 * @brief Rust library for stepper motor control using STM32L4 microcontroller
 * @details This file contains the implementation of the Stepper struct.
 *          The Stepper struct is used to control a stepper motor using the STM32L4 microcontroller.
 *          The stepper motor can be moved a certain number of degrees at a certain speed.
 *
 * @author Sungsu Kim
 * @date 2025-01-11
 * @version 1.1.0
 */

use hal::gpio::gpioa::PA4;
use hal::gpio::gpioa::PA5;
use hal::gpio::gpioa::PA6;
use hal::gpio::gpioa::PA7;
use hal::gpio::Output;
use hal::gpio::PushPull;
use hal::time::{Instant, MonoTimer};
use stm32l4xx_hal as hal;

use crate::print;
use crate::serial_sender::SENDER;

/**
 * Enum for the stepper motor sequence.
 */
enum StepperSeq {
    Seq1,
    Seq2,
    Seq3,
    Seq4,
}

/**
* Struct for controlling the stepper motor.
*/
pub struct Stepper {
    // infrastructures
    a_pos: PA4<Output<PushPull>>,
    a_neg: PA5<Output<PushPull>>,
    b_pos: PA6<Output<PushPull>>,
    b_neg: PA7<Output<PushPull>>,
    timer: MonoTimer,

    // interal states & parameters for a move
    steps_move: u32,
    step_interval_ms: f32,
    move_dir_cw: bool,
    ready_move_params: bool,
    stepper_seq: StepperSeq,
    time: Instant,
    time_total: Instant,
    is_moving: bool,
}

/**
* Implementation of the Stepper struct.
*/
impl Stepper {
    pub fn new(
        a_pos: PA4<Output<PushPull>>,
        a_neg: PA5<Output<PushPull>>,
        b_pos: PA6<Output<PushPull>>,
        b_neg: PA7<Output<PushPull>>,
        timer: MonoTimer,
    ) -> Self {
        Stepper {
            a_pos,
            a_neg,
            b_pos,
            b_neg,
            timer,
            steps_move: 0,
            step_interval_ms: 0.0,
            move_dir_cw: false,
            ready_move_params: false,
            stepper_seq: StepperSeq::Seq1,
            time: timer.now(),
            time_total: timer.now(),
            is_moving: false,
        }
    }

    /**
     * Set the parameters for the stepper motor to move a certain number of degrees at a certain speed.
     *
     * @param degrees: The number of degrees to move the stepper motor.
     * @param speed_percent: The speed at which to move the stepper motor as a percentage of the maximum speed.
     * @param sender: A mutable reference to the `SerialSender` used to send responses.
     */
    pub fn set_parameters(
        &mut self,
        degrees: f32,
        speed_percent: f32,
    ) -> bool {
        const MAX_STEPS: f32 = 2048.0;
        const MIN_TIME_EACH_STEP_MS: u32 = 3;
        const MAX_RPM: f32 = 60f32 / (MAX_STEPS * MIN_TIME_EACH_STEP_MS as f32 * 0.001);

        self.ready_move_params = false;

        if degrees == 0.0 {
            return false;
        }

        // steps to move - (+) for CW, (-) for CCW
        let steps = degrees / 360.0 * MAX_STEPS;
        let steps = steps as i32;
        self.steps_move = if steps > 0 {
            steps as u32
        } else {
            -steps as u32
        };
        print!("Stepper: deg={} steps={}\r\n", degrees, steps);

        // direction
        self.move_dir_cw = steps >= 0;

        // speed
        let mut percent = speed_percent;
        if speed_percent == 0.0 || speed_percent > 100.0 {
            percent = 100.0;
        }
        print!("Stepper: speed={}%, RPM={:.2}\r\n", percent, MAX_RPM * percent / 100f32 );

        self.step_interval_ms = MIN_TIME_EACH_STEP_MS as f32 * 100f32 / percent;
        self.ready_move_params = true;

        true
    }

    /**
     * Move the stepper motor based on the parameters set by the `set_parameters` function.
     *
     * @param sender: A mutable reference to the `SerialSender` used to send responses.
     * @return: `true` if the stepper motor has moved, `false` otherwise.
     */
    pub fn move_wait(&mut self) -> bool {
        let mut time = self.timer.now();
        let time_move = self.timer.now();

        let ticks_per_ms = self.timer.frequency().to_Hz() / 1000;
        let ms_to_ticks = |ms: u32| -> u32 { ticks_per_ms * ms };

        print!("Stepper: move...\r\n");
        while self.steps_move > 0 {
            self.run_stepper_sequence();

            loop {
                if time.elapsed() > ms_to_ticks(self.step_interval_ms as u32) {
                    time = self.timer.now();
                    break;
                }
            }
            self.steps_move -= 1;
        }
        print!("Stepper: ...done, elapsed={}ms\r\n", time_move.elapsed() / ms_to_ticks(1));

        self.ready_move_params = false;
        true
    }

    /**
     * Stop the stepper motor and reset the internal states.
     */
    pub fn stop_and_reset(&mut self) {
        self.steps_move = 0;
        self.ready_move_params = false;
        self.is_moving = false;
        print!("Stepper: stop and reset\r\n");
    }

    /**
     * Check if the stepper motor is moving.
     */
    pub fn is_moving(&self) -> bool {
        self.is_moving
    }

    /**
     * Task function to move the stepper motor in the main by being periodically called based on the parameters set by the `set_parameters` function.
     *
     * @param sender: A mutable reference to the `SerialSender` used to send responses.
     */
    pub fn run_task(&mut self) {
        if !self.ready_move_params {
            return;
        }

        let ticks_per_ms = self.timer.frequency().to_Hz() / 1000;
        let ms_to_ticks = |ms: u32| -> u32 { ticks_per_ms * ms };

        if self.steps_move == 0 {
            print!("Stepper: ...move async done, elapsed={}ms\r\n", self.time_total.elapsed() / ms_to_ticks(1));
            self.ready_move_params = false;
            self.is_moving = false;
            return;
        }

        if self.time.elapsed() > ms_to_ticks(self.step_interval_ms as u32) {
            if !self.is_moving {
                print!("Stepper: move async...\r\n");
                self.time_total = self.timer.now();
                self.is_moving = true;
            }

            self.run_stepper_sequence();
            self.steps_move -= 1;
            self.time = self.timer.now();
        }
    }

    /**
     * Run the stepper motor sequence based on the current sequence state.
     *
     * @return: The next sequence state.
     */
    fn run_stepper_sequence(&mut self) {
        let dir_cw = self.move_dir_cw;
        let _sequence_next: StepperSeq;

        match self.stepper_seq {
            StepperSeq::Seq1 => {
                self.a_pos.set_high();
                self.a_neg.set_low();
                self.b_pos.set_low();
                self.b_neg.set_low();
                self.stepper_seq = if dir_cw == true {
                    StepperSeq::Seq4
                } else {
                    StepperSeq::Seq2
                }
            }
            StepperSeq::Seq2 => {
                self.a_pos.set_low();
                self.a_neg.set_high();
                self.b_pos.set_low();
                self.b_neg.set_low();
                self.stepper_seq = if dir_cw == true {
                    StepperSeq::Seq1
                } else {
                    StepperSeq::Seq3
                }
            }
            StepperSeq::Seq3 => {
                self.a_pos.set_low();
                self.a_neg.set_low();
                self.b_pos.set_high();
                self.b_neg.set_low();
                self.stepper_seq = if dir_cw == true {
                    StepperSeq::Seq2
                } else {
                    StepperSeq::Seq4
                }
            }
            StepperSeq::Seq4 => {
                self.a_pos.set_low();
                self.a_neg.set_low();
                self.b_pos.set_low();
                self.b_neg.set_high();
                self.stepper_seq = if dir_cw == true {
                    StepperSeq::Seq3
                } else {
                    StepperSeq::Seq1
                }
            }
        };
    }
}
