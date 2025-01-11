#![no_main]
#![no_std]

extern crate cortex_m;
extern crate cortex_m_rt as runtime;
extern crate stm32l4xx_hal;

#[macro_use(block)]
extern crate nb;

use core::panic::PanicInfo;
use cortex_m_rt::entry;
use stm32l4xx_hal as hal;
use hal::prelude::*;
use hal::serial::{Serial, Tx};
use hal::pac::USART2;
use hal::time::MonoTimer;
use hal::gpio::gpiob::PB3;
use hal::gpio::gpioa::PA4;
use hal::gpio::gpioa::PA5;
use hal::gpio::gpioa::PA6;
use hal::gpio::gpioa::PA7;
use hal::gpio::Output;
use hal::gpio::PushPull;
use heapless::String;
use heapless::Vec;
use core::fmt::Write;

const SIZE_TX_BUFFER: usize = 128;
const SIZE_RX_BUFFER: usize = 64;
const MAX_ARGS: usize = 20;

type FixedStringSlices = Vec<String<SIZE_RX_BUFFER>, MAX_ARGS>;

#[entry]
/**
 * Main function.
 */
fn main() -> ! {
    let ( timer, mut gpio_led3, mut gpio_serial_tx, mut gpio_serial_rx, gpio_stepper_a_pos, gpio_stepper_a_neg, gpio_stepper_b_pos, gpio_stepper_b_neg) = init_hardware();

    // serial input 
    let mut sender = SerialSender::new(gpio_serial_tx);
    let mut str_buffer: String<SIZE_RX_BUFFER> = String::new();
    str_buffer.clear();

    // timer
    let mut time_tick = timer.now();
    
    // stepper motor
    let mut stepper = Stepper::new(gpio_stepper_a_pos, gpio_stepper_a_neg, gpio_stepper_b_pos, gpio_stepper_b_neg, timer.clone() );

    // start the service
    cli_print_info(&mut sender);
    print!(sender, "Enter a command...\r\n");

    loop {
        let get_tick_ms = |ms: u32| -> u32 { 
            timer.frequency().to_Hz() / 1000 * ms
        };

        if time_tick.elapsed() > get_tick_ms(1000) {
            time_tick = timer.now();

            // toggle led
            gpio_led3.toggle();
        }

        if let Some(slices) = get_command_slices(&mut gpio_serial_rx, &mut str_buffer) {
            print!(sender, "CLI: #args={}\r\n", slices.len());
    
            let command = slices.get(0).unwrap();
            match command.as_str() {
                "info" => {
                    cli_print_info(&mut sender);
                }
                "led" => {
                    cli_led(slices.clone(), &mut sender, &mut gpio_led3);
                }
                "stepper" => {
                    cli_stepper(slices.clone(), &mut sender, &mut stepper);
                }
                _ => {
                    print!(sender, "CLI: undefined command ({})\r\n", command.as_str());
                }
            }
        }
    }
}

/**
 * This function prints the welcome message and project information to the serial sender.
 *
 * @param sender: A mutable reference to the `SerialSender` used to send the information.
 */
fn cli_print_info(sender: &mut SerialSender<Tx<USART2>>)
{
    print!(sender, "************************************\r\n");
    print!(sender, "* Welcome to STM32L431 Rust Project\r\n");
    print!(sender, "* Version: 1.1.0\r\n");
    print!(sender, "* Author: SSKIM \r\n");
    print!(sender, "************************************\r\n");
}

/**
 * This function controls the state of an LED based on the command received.
 * The command is expected to be in the first slice of the `slices` vector.
 * If the command is "on", the LED is set to high. Otherwise, the LED is set to low.
 *
 * @param slices: A vector of `String<SIZE_RX_BUFFER>`, each representing a slice of the received command.
 * @param sender: A mutable reference to the `SerialSender` used to send responses.
 * @param led: A mutable reference to the LED to be controlled.
 */
fn cli_led(slices: FixedStringSlices, sender: &mut SerialSender<Tx<USART2>>, led: &mut PB3<Output<PushPull>>)
{
    if let Some(status) = slices.get(1) {
        print!(sender, "CLI: led {}\r\n", status.as_str());
        if status.as_str() == "on" {
            led.set_high();
        }
        else {
            led.set_low();
        }
    }
}

fn cli_stepper( slices: FixedStringSlices, 
                sender: &mut SerialSender<Tx<USART2>>, 
                stepper: &mut Stepper
                )
{
    if let Some(degrees) = slices.get(1) {

        let move_degrees = degrees.parse().unwrap_or(0f32);
        if move_degrees == 0f32 {
            print!(sender, "Stepper: error, zero degree\r\n");
            return;
        }

        let mut speed_percent = 100f32;
        if let Some(speed) = slices.get(2) {
            speed_percent = speed.parse().unwrap_or(0f32);
        }

        print!(sender, "CLI: Stepper move {:.2} degrees\r\n", move_degrees);
        print!(sender, "CLI: Stepper speed {:.2}%\r\n", speed_percent);

        stepper.set_parameters(move_degrees, speed_percent);
        stepper.move_wait();
    }
}

/**
 * This function iterates over the slices of the received command and prints each slice to the serial sender.
 *
 * @param slices: A vector of `String<SIZE_RX_BUFFER>`, each representing a slice of the received command.
 * @param sender: A mutable reference to the `SerialSender` used to send responses.
 */
fn cli_peek_slices(slices: FixedStringSlices, sender: &mut SerialSender<Tx<USART2>>)
{
    for slice in slices {
        let maybe_num: Result<i32, _> = slice.parse();
        match maybe_num {
            Ok(num) => {
                print!(sender, "arg(num): {}\r\n", num);
            }
            Err(_) => {
                print!(sender, "arg(str): {}\r\n", slice);
            }
        }
    }
}

/**
 * Enum for the stepper motor sequence.
 */
enum StepperSeq{
    Seq1,
    Seq2,
    Seq3,
    Seq4
}

/**
 * Struct for controlling the stepper motor.
 */
struct Stepper {
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
}

/**
 * Implementation of the Stepper struct.
 */
impl Stepper {
    pub fn new(a_pos: PA4<Output<PushPull>>, a_neg: PA5<Output<PushPull>>, b_pos: PA6<Output<PushPull>>, b_neg: PA7<Output<PushPull>>, timer: MonoTimer ) -> Self {
        Stepper {
            a_pos, a_neg, b_pos, b_neg,
            timer,
            steps_move: 0,
            step_interval_ms: 0.0,
            move_dir_cw: false,
            ready_move_params: false,
            stepper_seq: StepperSeq::Seq1
        }
    }

    /**
     * Set the parameters for the stepper motor to move a certain number of degrees at a certain speed.
     */
    pub fn set_parameters(&mut self, degrees: f32, speed_percent: f32 ) -> bool {
        
        const MAX_STEPS: f32 = 2048.0;
        const MIN_TIME_EACH_STEP_MS: u32 = 3;
        const MAX_RPM: f32 = 60f32 / ( MAX_STEPS * MIN_TIME_EACH_STEP_MS as f32 * 0.001 );

        self.ready_move_params = false;

        if degrees == 0.0 {
            return false;
        }
        
        // steps to move - (+) for CW, (-) for CCW
        let steps = degrees / 360.0 * MAX_STEPS;
        let steps = steps as i32;
        self.steps_move = if steps > 0 { steps as u32 } else { -steps as u32 };

        // direction
        self.move_dir_cw = steps >= 0;

        // speed
        let mut percent = speed_percent;
        if speed_percent == 0.0 || speed_percent > 100.0 {
            percent = 100.0;
        }

        self.step_interval_ms = MIN_TIME_EACH_STEP_MS as f32 * 100f32 / percent;
        self.ready_move_params = true;

        //print!(sender, "Stepper: deg={} steps={}\r\n", move_degrees, steps);
        //print!(sender, "Stepper: speed={}%, RPM={:.2}\r\n", speed_percent, MAX_RPM * speed_percent / 100f32);    

        true
    }

    /**
     * Move the stepper motor based on the parameters set by the `set_parameters` function.
     */
    pub fn move_wait(&mut self) -> bool { 

        let mut time = self.timer.now();

        while self.steps_move > 0 {
            self.run_stepper_sequence();

            loop {
                let get_tick_ms = |ms: u32| -> u32 { self.timer.frequency().to_Hz() / 1000 * ms };
                if time.elapsed() > get_tick_ms(self.step_interval_ms as u32) {
                    time = self.timer.now();
                    break;
                }
            }
            self.steps_move -= 1;
        }

        self.ready_move_params = false;
        true
    }

    /**
     * Run the stepper motor sequence based on the current sequence state.
     */
    fn run_stepper_sequence(&mut self) {
        
        let dir_cw = self.move_dir_cw;
        let sequence_next: StepperSeq;

        match self.stepper_seq {
            StepperSeq::Seq1 => {
                self.a_pos.set_high();
                self.a_neg.set_low();
                self.b_pos.set_low();
                self.b_neg.set_low();
                self.stepper_seq = if dir_cw == true { StepperSeq::Seq4 } else { StepperSeq::Seq2 }
            }
            StepperSeq::Seq2 => {
                self.a_pos.set_low();
                self.a_neg.set_high();
                self.b_pos.set_low();
                self.b_neg.set_low();
                self.stepper_seq = if dir_cw == true { StepperSeq::Seq1 } else { StepperSeq::Seq3 }
            }
            StepperSeq::Seq3 => {
                self.a_pos.set_low();
                self.a_neg.set_low();
                self.b_pos.set_high();
                self.b_neg.set_low();
                self.stepper_seq = if dir_cw == true { StepperSeq::Seq2 } else { StepperSeq::Seq4 }
            }
            StepperSeq::Seq4 => {
                self.a_pos.set_low();
                self.a_neg.set_low();
                self.b_pos.set_low();
                self.b_neg.set_high();
                self.stepper_seq = if dir_cw == true { StepperSeq::Seq3 } else { StepperSeq::Seq1 }
            }
        };
    }
}

/**
 * This function reads from the serial receiver and splits the received string into slices based on spaces.
 * Each slice is then converted into a `String<SIZE_RX_BUFFER>` and stored in a `Vec`.
 * The function returns `None` if no data is received from the serial receiver, or if the received data does not end with a newline character.
 * If the received data ends with a newline character, the function returns a `Vec` of `String<SIZE_RX_BUFFER>`, each representing a slice of the received data.
 *
 * @param serial_rx: A mutable reference to the serial receiver.
 * @param str_buffer: A mutable reference to a `String<SIZE_RX_BUFFER>` that is used as a buffer to store the received data.
 * @return: An `Option` that contains a `Vec` of `String<SIZE_RX_BUFFER>` if the received data ends with a newline character, or `None` otherwise.
 */
fn get_command_slices( serial_rx: &mut hal::serial::Rx<USART2>, 
                       str_buffer: &mut String<SIZE_RX_BUFFER>) -> Option<FixedStringSlices> {
                        
    let received = serial_rx.read().ok();
 
    if let Some(ch) = received {
        if ch != b'\n' {
            // push byte into the string buffer
            str_buffer.push(ch as char).unwrap();
            None
        } else {
            // NOTE: This is in order to fix a problem with the last number to be treated as a string when parsed.
            str_buffer.pop();
            str_buffer.push(' ').unwrap();

            let mut slices: FixedStringSlices = Vec::new();
            let mut start = 0;

            for (index, char) in str_buffer.char_indices() {
                if char == ' ' {
                    if start != index {
                        let slice_str = &str_buffer[start..index];
                        let mut slice_string: String<SIZE_RX_BUFFER> = String::new();
                        slice_string.push_str(slice_str).unwrap();
                        let _ = slices.push(slice_string);
                    }
                    start = index + 1;
                }
            }

            if start < str_buffer.len() {
                let slice_str = &str_buffer[start..];
                let mut slice_string: String<SIZE_RX_BUFFER> = String::new();
                slice_string.push_str(slice_str).unwrap();
                let _ = slices.push(slice_string);
            }

            str_buffer.clear();
            Some(slices)
        }
    } else {
        None
    }
}

/**
 * Trait for sending bytes.
 */
trait SendByte {
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
struct SerialSender<T: SendByte> {
    tx: T,
}

/**
 * Implementation of the SerialSender trait.
 */
impl<T: SendByte> SerialSender<T> {
    fn new(tx: T) -> Self {
        SerialSender { tx }
    }

    fn send_formatted(&mut self, format: core::fmt::Arguments) {
        let mut buffer = heapless::String::<SIZE_TX_BUFFER>::new(); // Adjust buffer size as needed
        write!(buffer, "{}", format).unwrap();
        self.tx.send_bytes(&buffer);
        buffer.clear();
    }
}

/**
 * Initialize the hardware.
 * 
 * @return The LED, TX, RX, and timer.
 */
fn init_hardware() -> (MonoTimer, PB3<Output<PushPull>>, Tx<USART2>, hal::serial::Rx<USART2>, PA4<Output<PushPull>>, PA5<Output<PushPull>>, PA6<Output<PushPull>>, PA7<Output<PushPull>>) {

    // Setup the clock and etc
    let p = hal::stm32::Peripherals::take().unwrap();
    let mut rcc = p.RCC.constrain();

    let mut flash = p.FLASH.constrain();
    let mut pwr = p.PWR.constrain(&mut rcc.apb1r1);

    let clocks = rcc.cfgr.sysclk(80.MHz()).pclk1(80.MHz()).pclk2(80.MHz()).freeze(&mut flash.acr, &mut pwr);

    // Setup GPIO pins
    let mut gpioa = p.GPIOA.split(&mut rcc.ahb2);
    let tx = gpioa.pa2.into_alternate(&mut gpioa.moder, &mut gpioa.otyper, &mut gpioa.afrl);
    let rx = gpioa.pa3.into_alternate(&mut gpioa.moder, &mut gpioa.otyper, &mut gpioa.afrl);

    let pin_stepper_a_pos = gpioa.pa4.into_push_pull_output(&mut gpioa.moder, &mut gpioa.otyper);
    let pin_stepper_a_neg = gpioa.pa5.into_push_pull_output(&mut gpioa.moder, &mut gpioa.otyper);
    let pin_stepper_b_pos = gpioa.pa6.into_push_pull_output(&mut gpioa.moder, &mut gpioa.otyper);
    let pin_stepper_b_neg = gpioa.pa7.into_push_pull_output(&mut gpioa.moder, &mut gpioa.otyper);

    let mut gpiob = p.GPIOB.split(&mut rcc.ahb2);
    let led = gpiob.pb3.into_push_pull_output(&mut gpiob.moder, &mut gpiob.otyper);

    // Setup the serial pins
    let serial = Serial::usart2(p.USART2, (tx, rx), 115_200.bps(), clocks, &mut rcc.apb1r1);
    let (tx, rx) = serial.split();

    // Setup the timer
    let mut cp = cortex_m::Peripherals::take().unwrap();
    cp.DCB.enable_trace();
    cp.DWT.enable_cycle_counter();
    let timer: MonoTimer = MonoTimer::new(cp.DWT, clocks);

    (timer, led, tx, rx, pin_stepper_a_pos, pin_stepper_a_neg, pin_stepper_b_pos, pin_stepper_b_neg)
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

/**
 * Panic handler.
 */
#[panic_handler]
fn panic(_panic: &PanicInfo<'_>) -> ! {
    loop {}
}
