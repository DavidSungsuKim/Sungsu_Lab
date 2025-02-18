use core::fmt::Write;
use defmt::debug;
use embassy_time::{Timer, Instant};
use embassy_stm32::gpio::{Output};
use crate::print;
use crate::cli::SERIAL_TX;

/**
 * @brief Enum for the stepper motor sequence.
 */
enum StepperSeq {
   Seq1,
   Seq2,
   Seq3,
   Seq4,
}

/**
 * @brief Stepper motor driver
 */
pub struct Stepper<'d> {
   a_pos: Output<'d>,
   a_neg: Output<'d>,
   b_pos: Output<'d>,
   b_neg: Output<'d>,

   // interal states & parameters for a move
   steps_move: u32,
   step_interval_ms: f32,
   move_dir_cw: bool,
   ready_move_params: bool,
   stepper_seq: StepperSeq,
   is_moving: bool,
}

impl<'d> Stepper<'d> {
   /**
    * @brief Create a new instance of the stepper motor driver.
    */
   pub fn new (
      a_pos: Output<'d>,
      a_neg: Output<'d>,
      b_pos: Output<'d>,
      b_neg: Output<'d>,
   ) -> Self {
      Self {
         a_pos,
         a_neg,
         b_pos,
         b_neg,

         steps_move: 0,
         step_interval_ms: 0.0,
         move_dir_cw: false,
         ready_move_params: false,
         stepper_seq: StepperSeq::Seq1,
         is_moving: false,
      }
   }

   /**
    * @brief Set the parameters for the stepper motor.
    * @param degrees: the degrees to move.
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
      debug!("Stepper: deg={} steps={}\r\n", degrees, steps);

      // direction
      self.move_dir_cw = steps >= 0;

      // speed
      let mut percent = speed_percent;
      if speed_percent == 0.0 || speed_percent > 100.0 {
          percent = 100.0;
      }
      debug!("Stepper: speed={}%, RPM={}\r\n", percent, MAX_RPM * percent / 100f32 );

      self.step_interval_ms = MIN_TIME_EACH_STEP_MS as f32 * 100f32 / percent;
      self.ready_move_params = true;

      true
   }

   /**
    * @brief Move the stepper motor relative to the current position.
    */
   pub async fn move_relative(&mut self) -> bool {
      let time_start = Instant::now();

      print!("Stepper: move...\r\n");
      while self.steps_move > 0 {
            self.run_stepper_sequence();

            Timer::after_millis(self.step_interval_ms as u64).await;
            self.steps_move -= 1;
      }

      let time_elapsed = time_start.elapsed();
      print!("Stepper: ...done, elapsed={}ms\r\n", time_elapsed.as_millis());

      self.ready_move_params = false;
      true
   }

   /**
    * @brief Stop the stepper motor and reset the internal states.
    */
   pub fn stop_and_reset(&mut self) {
      self.steps_move = 0;
      self.ready_move_params = false;
      self.is_moving = false;
      debug!("Stepper: stop and reset\r\n");
   }

   /**
    * @brief Helper to check if the stepper motor is moving.
    * @return: true if the stepper motor is moving.
    */
   pub fn is_moving(&self) -> bool {
      self.is_moving
   }

   /**
    * @brief Run the stepper motor sequence based on the current sequence state.
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