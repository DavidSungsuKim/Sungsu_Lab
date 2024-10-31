// -------------------------------------------------
// 			Declarative Macros
// -------------------------------------------------

/* General Syntax
macro_rules! macro_name {
//   |--- Match rules
    (...) => { ... };
    (...) => { ... };
    (...) => { ... };    // the semicolon at the last rule is optional
}
*/

/*
macro_rules! our_macro {
    () => {
        1 + 1;
    };

    (something 4 u dear u32 @_@) => {
        println!("You found nonsense here")
    };

    ($e1:expr, $e2:expr) => {
        $e1 + $e2
    };

    ($a:expr, $b:expr; $c:expr) => {
        $a * ($b + $c)
    };
}
fn main() {
    println!("{}", our_macro!());
    our_macro!();
    println!("{}", our_macro!(2, 2));
    println!("{}", our_macro!(5,6;3));
    // println!("{}", our_macro!("something",2;"nothing"));

    our_macro!();
    our_macro![];

    our_macro! {};

    // 1. cargo install cargo-expand
    // 2. rustup install nightly
    // 3. rustup component add rustfmt
    // 4. rustup component add rustfmt --toolchain nightly
}
*/

// -------------------------------------------------
// 			Capturing Types
// -------------------------------------------------
/*
macro_rules! input {
    ($t: ty) => {{
        let mut n = String::new();
        std::io::stdin()
            .read_line(&mut n)
            .expect("failed to read input");

        let n: $t = n.trim().parse().expect("invalid input");
        n
    }};
}

macro_rules! add_as {
    ($a: expr, $b: expr, $typ: ty) => {
        $a as $typ + $b as $typ
    };
}

macro_rules! some_macro {
    ($var: ident) => {
        $var = $var + 1;
    };
}

fn main() {
    /*
    println!("Please enter a floating point number");
    let some_input_0 = input!(f32);
    */

    // println!("{}", add_as!(15,2.3,f32));
    let mut x = 4;
    some_macro!(x);
}
*/

/*
macro_rules! create_function {
    ($func_name:ident, $input: ident, $type_input: ty, $type_output: ty) => {
        fn $func_name($input: $type_input) -> $type_output {
            println!(
                "You called {:?}() with the input of {:?}",
                stringify!($func_name),
                stringify!($input1)
            );
            $input
        }
    };
}

create_function!(f1, x, i32, i32);
fn main() {
    //f1(15);
    let y = f1(15);
}
*/

// -------------------------------------------------
//          	- Repeating Patterns
// -------------------------------------------------
/*
macro_rules! string_concat {
    // () => {
    //     String::new();
    // };

    // ($some_str: expr) => {{
    //     let mut temp_str = String::new();
    //     temp_str.push_str($some_str);
    //     temp_str
    // }
    // };

    // ($some_s1: expr, $some_s2:expr) => {{
    //     let mut temp_str = String::new();
    //     temp_str.push_str($some_s1);
    //     temp_str.push_str($some_s2);

    //     temp_str
    // }
};

($($some_str:expr,) *) => {{
    let mut temp_str = String::new();
    $(temp_str.push_str($some_str);)*
    temp_str
}
};

}

macro_rules! string_concat {
    ($($some_str:expr),* ) => {{
        let mut temp_str = String::new();
        $(temp_str.push_str($some_str);)*
        temp_str
    }};
}

fn main() {
    let str_null = string_concat!();
    let str_single = string_concat!("First");
    let str_double = string_concat!("First", "Second");
    println!("{}", str_null); // 출력: ""
    println!("{}", str_single); // 출력: "First"
    println!("{}", str_double); // 출력: "FirstSecond"
}
*/

/*
// homework 1.
// We want to create a macro called make_struct which will create a new struct containing some fields. The input to the macro is the name of the struct and the name of the fields along with their types. The skeleton of the macro along with its left sides of the rules are given.
// You are required to write the code for the expansion or the right side of the rule.

macro_rules! make_struct {
    ($name:ident {$($field:ident: $ty:ty),* }) => {
       // Your code here
        #[derive(Debug)]
        struct $name {
            $($field: $ty),*
        };
    };

}
fn main() {
    make_struct!(Person {
        name: String,
        age: u32,
        occupation: String
    });

    let p = Person {
        name: "Senior".to_string(),
        age: 83,
        occupation: "embedded developer".to_string(),
    };

    println!("{:?}", p);
}
*/

/*
// homework 2.
// Consider the code below. Show the expansion part of this code, espacially for the invocation to macro.
macro_rules! make_functions {
    ($($func_name:ident: $return_type:ty => $return_expr:expr),+) => {
        $(
            fn $func_name() -> $return_type {

                $return_expr
            }
        )+
    };
}

make_functions!(foo: i32 => 42, bar: String => "hello world".to_owned());

fn main() {
    let result1 = foo();
    let result2 = bar();

    println!("foo result: {}", result1);
    println!("bar result: {}", result2);
}*/

/*
// answer:
fn main() {
    let result1 = foo();
    let result2 = bar();
    {
        ::std::io::_print(format_args!("foo result: {0}\n", result1));
    };
    {
        ::std::io::_print(format_args!("bar result: {0}\n", result2));
    };
}
*/

/*
// homework 3.
// Consider the code below. Write the expanded version of the code that can be viewed using the cargo expand utility.

macro_rules! sum_macro {
    ($($x:expr),*) => {
        {
            let mut sum = 0;
            $(sum += $x;)*
            sum
        }
    };
}

fn main() {
    let result = sum_macro!(1, 2, 3, 4, 5);
}
*/

/*
// answer:
fn main() {
    let result = {
        let mut sum = 0;
        sum += 1;
        sum += 2;
        sum += 3;
        sum += 4;
        sum += 5;
        sum
    };
}
*/

// -------------------------------------------
// 			Question Marks Operator
// -------------------------------------------
/*
use std::num::ParseIntError;

fn parse_string(input_str: &str) -> Result<i32, ParseIntError> {
    let parsed = input_str.parse::<i32>()?;
    Ok(parsed)
}

fn main() {
    let values = vec!["123", "some1", "some(123)", "abc", "53"];
    for value in values {
        println!("{:?}", parse_string(value));
    }
}*/

/*
// example when there's no '?' operators
use std::num::ParseIntError;

fn parse_string(input_str: &str) -> Result<i32, ParseIntError> {
    match input_str.parse::<i32>() {
        Ok(parsed) => Ok(parsed),
        Err(e) => Err(e),
    }
}

fn main() {
    let values = vec!["123", "some1", "some(123)", "abc", "53"];
    for value in values {
        match parse_string(value) {
            Ok(parsed) => println!("Parsed value: {}", parsed),
            Err(e) => println!("Failed to parse '{}': {:?}", value, e),
        }
    }
}
*/

/*
fn division(dividend: f64, divisor: f64) -> Result<f64, String> {
    //fn division(dividend: f64, divisor: f64) -> Result<f64, u8>  {

    let answer = match divisor {
        0.0 => Err(String::from("Error: Division by zero")), // Err(8),
        _ => Ok(dividend / divisor),
    };

    let correct = answer?; // it is also going to unwrap the result
                           // A small note: this operator will take the ownership
                           //println!("{:?}",answer);
    println!("This line will not print in case of error {:?}", correct);
    Ok(correct)
}

fn main() {
    println!(
        "Call from main with result equals to {:?}\n",
        division(9.0, 3.0)
    );
    println!(
        "Call from main with result equals to {:?}\n",
        division(4.0, 0.0)
    );
    println!(
        "Call from main with result equals to {:?}\n",
        division(0.0, 2.0)
    );
}
*/

/*
fn division(dividend: f64, divisor: f64) -> Option<f64> {
    let answer = match divisor {
        0.0 => None,
        _ => Some(dividend / divisor),
    };

    let correct = answer?; // A small note: this operator will take the ownership

    // println!("{:?}",answer);
    println!("This line will not print in case of error {:?}", correct);
    Some(correct)
}

fn main() {
    //let x = division(1.0,2.0);
    println!(
        "Call from main with result equals to {:?}\n",
        division(9.0, 3.0)
    );
    println!(
        "Call from main with result equals to {:?}\n",
        division(4.0, 0.0)
    );
    println!(
        "Call from main with result equals to {:?}\n",
        division(0.0, 2.0)
    );
}
*/

/*
#[derive(Debug)]
enum MathError {
    DivisionError_DivisionByZero,
    LogError_NonPositiveLogarithm,
    SqrtError_NegativeSquareRoot,
}

type MathResult = Result<(), MathError>;

fn division(x: f64, y: f64) -> MathResult {
    if y == 0.0 {
        Err(MathError::DivisionError_DivisionByZero)
    } else {
        println!("The division is successful and has a result of {}", x / y);
        Ok(())
    }
}

fn sqrt(x: f64) -> MathResult {
    if x < 0.0 {
        Err(MathError::SqrtError_NegativeSquareRoot)
    } else {
        println!(
            "The square root is successful and has a result of {}",
            x.sqrt()
        );
        Ok(())
    }
}

fn ln(x: f64) -> MathResult {
    if x <= 0.0 {
        Err(MathError::LogError_NonPositiveLogarithm)
    } else {
        println!("The log was successful and has a result of {}", x.ln());
        Ok(())
    }
}
fn operations(x: f64, y: f64) -> MathResult {
    division(x, y)?;
    sqrt(x)?;
    ln(x)?;
    Ok(())
}

fn main() {
    let result = operations(0.0, 10.0);
    if result.is_ok() {
        println!("All the functions executed successfully");
    } else {
        println!("{:?}", result);
    }
}
*/

// Another example not in the video lectures but simple to understand and very useful
use std::env;

fn current_dir1() -> std::io::Result<()> {
    let path = env::current_dir()?; // for unwrapping the current path
    println!("\n\n The current path is {:?}\n\n ", path);
    Ok(())
}

fn main() {
    current_dir1();
}
