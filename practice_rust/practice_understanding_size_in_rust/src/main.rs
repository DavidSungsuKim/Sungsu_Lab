/*
// -------------------------------------------
// 			- Size in Rust
// 				- Sized Types
//				- Unsized Types
// -------------------------------------------
use std::mem::size_of;
trait Some_trait {}
fn main() {
    // Sized Types
    println!("i32 size is: {}", size_of::<i32>());
    println!("(i32,i32) size is: {}", size_of::<(i32, i32)>());
    println!("[i32: 3] size is: {}", size_of::<[i32; 3]>());

    struct Point {
        x: bool,
        y: i64,
    }
    println!("Struct size is: {}", size_of::<Point>());
    println!("i32 reference is: {}", size_of::<&i32>());
    println!("i32 mutable reference is: {}", size_of::<&mut i32>());
    println!("Machine word size is: {}", size_of::<&()>());
    println!("Box<i32> is: {}", size_of::<Box<i32>>());
    println!("fn(i32) -> i32 is: {}", size_of::<fn(i32) -> i32>());

    // Unsized Types
    println!("[i32] size is: {}", size_of::<&[i32]>());
    let a: [i32; 3];
    //println!("str size is: {}", size_of::<str>());
    println!(
        "The size of the trait object is: {}",
        size_of::<&dyn Some_trait>()
    );
}
*/

/*
// -------------------------------------------
// 		- Pointers to Sized vs Unsized Types
// -------------------------------------------

use std::mem::size_of;
trait Shape {
    fn print(&self);
}

#[derive(Debug)]
struct Circle;

#[derive(Debug)]
struct Rectangle;

impl Shape for Circle {
    fn print(&self) {
        println!("{:?}", self);
    }
}

impl Shape for Rectangle {
    fn print(&self) {
        println!("{:?}", self);
    }
}
fn main() {
    println!(
        "Size of a reference to sized type: {}",
        size_of::<&[i32; 3]>()
    );
    println!(
        "Size of a reference to unsized type: {}",
        size_of::<&[i32]>()
    );

    let num_1: &[i32; 3] = &[10, 12, 30];
    let num_2: &[i32] = &[10, 12, 30];

    let mut sum = 0;
    for num in num_1 {
        sum += num;
    }

    for num in num_2 {
        sum += num;
    }

    println!("Size of &Cricle is: {}", size_of::<&Circle>());
    println!("Size of &Rectangle is: {}", size_of::<&Rectangle>());
    println!("Size of &dyn Shape: {}", size_of::<&dyn Shape>());
}
*/

/*
// -------------------------------------------
// 	- Sized Trait
//      - Optionally Sized Trait
// -------------------------------------------

// include as dependency in the cargo.toml file  negative-impl = "0.1.4"

use negative_impl::negative_impl;
struct ABC;

#[negative_impl]
impl !Send for ABC {}

#[negative_impl]
impl !Sync for ABC {}

// #[negative_impl]
// impl !Sized for ABC {}

//fn some_fn<T>(t: T) {}
//fn some_fn<T: Sized>(t: T) {} // this doesn't compile.
fn some_fn<T: ?Sized>(t: &T) {}

fn main() {
    let x: i32 = Default::default();
    let y = Clone::clone(&x);
}
*/

/*
// -------------------------------------------
// 		- ?Sized and Generic Parameters
// -------------------------------------------

use std::fmt::Debug;

// 1. Must have a single unsized field.
// 2. The unsized field must be the last field.
struct UnSizedStruct<T: ?Sized> {
    sized_field_1: i32,
    unsized_field: T,
}

// fn print_fn<T: Debug>(t: T)
fn print_fn<T: Debug + ?Sized>(t: &T) {
    println!("{:?}", t);
}

fn main() {
    let x = UnSizedStruct {
        sized_field_1: 3,
        unsized_field: [3],
    };

    let x = "my name";
    print_fn(&x);

    //     /*
    //     Parameter type          T	        &T          &T
    //     Function call Input     &str	        &str        &&str
    //     Resolves to             T = &str	    T = str     T = &str
    //     */
}
*/

/*
// -------------------------------------------
// 			- Unsized Coercion
// -------------------------------------------

fn str_slice_fn(s: &str) {}

fn array_slice_fn<T>(s: &[T]) {}

trait Some_Trait {
    fn method(&self);
}

impl<T> Some_Trait for [T] {
    fn method(&self) {}
    // can now call "method" on
    // 1) any &[T]
    // 2) Vec<T>
    // 3) [T; N]
}
fn main() {
    let some_string = String::from("String");
    str_slice_fn(&some_string);

    let slice: &[i32] = &[1];
    let vec = vec![1];
    let array = [1, 2, 3];

    array_slice_fn(slice);
    array_slice_fn(&vec); // deref coercion
    array_slice_fn(&array); // Unsized coercion

    slice.method();
    vec.method(); // deref coercion
    array.method(); // Unsized coercion
}
*/

/*
// -------------------------------------------
// 			- Zero Sized Types
// 		        - Never Type
// -------------------------------------------
// to change to nightly use the command of rustup override set nightly
// to return to the original one --> rustup override set stable
#![feature(never_type)]

fn unrecoverable_state() -> ! {
    panic!("This function will never return normally with something valid");
}

// fn function() -> Result<i32, String> {}
// fn function_1() -> Result<i32, !> {}
// fn function_2() -> Result<!, String> {}

// fn function() -> Result<NeverType, String> {}
// fn function_1() -> Result<i32, NeverType> {}
enum NeverType {}

fn main() {
    // unrecoverable_state();
    //let x = !;
    //let x = unrecoverable_state();
    let x: !;

    let x = match "123".parse::<i32>() {
        Ok(num) => num,
        Err(_) => panic!(),
    };

    let x: String = return;
    let counter = 0;
    let result = loop {
        counter += 1;
        if counter == 10 {
            break;
        }
    };

    //let x = NeverType;
}
*/

/*
// -------------------------------------------
// 			- Zero Sized Types
// 			    - Unit Type
// -------------------------------------------
// fn f1() {}
fn f1() -> () {}
fn division_status(divident: f64, divisor: f64) -> Result<(), String> {
    let answer = match divisor {
        0.0 => Err(String::from("Error: Division by zero")),
        _ => {
            println!("The division is invalid");
            Ok(())
        }
    };
    answer
}
fn main() {
    let x = ();
    let y = f1();

    let z = println!("Hello, world!");

    let mut vec: Vec<()> = Vec::with_capacity(0);
    vec.push(());
    vec.push(());
    vec.push(());
    assert_eq!(3, vec.len());
    println!("{}", vec.capacity());

    /*
    Unit Type                          || Never Types
    1. No meaningful value             || 1. Never produces a value
    2. Function returning unit         || 2. Function returning never, will never
    always returns normally            || returns normally
    3. single value, which can not be  || 3. No associated value, and can be coerced
    coerced                            || to all types.
    */
}
*/

/*
// -------------------------------------------
// 		- Zero Sized Types
// 		- Unit Struct
// -------------------------------------------

struct Admin;
struct User;

trait Authenticate {
    fn authenticate(&self, username: &str, password: &str) -> bool;
}

impl Authenticate for Admin {
    fn authenticate(&self, username: &str, password: &str) -> bool {
        username == "admin" && password == "adminpass"
    }
}

impl Authenticate for User {
    fn authenticate(&self, username: &str, password: &str) -> bool {
        username == "user" && password == "userpass"
    }
}

fn login<T: Authenticate>(role: T, username: &str, password: &str) -> bool {
    role.authenticate(username, password)
}

fn main() {
    let admin = Admin;
    let user = User;

    let admin_login = login(admin, "admin", "adminpass");
    let user_login = login(user, "user", "userpass");

    if admin_login {
        println!("Admin login successful!");
    } else {
        println!("Admin login failed!");
    }

    if user_login {
        println!("User login successful!");
    } else {
        println!("User login failed!");
    }
    struct ABC;
    let x = ();
    let y = x;
    let z = x;

    let x = ABC;
    let y = x;
    //let z = x;
}

// Extra examples:
// // Type-Level Constraints: Unit structs can be used as type-level constraints to enforce certain conditions or behaviors at the type level. For example, you can define a unit struct called NonEmpty to enforce that a collection or container type must always have at least one element.
// #[derive(Debug)]
// struct NonEmpty<T>(T);

// impl<T> NonEmpty<Vec<T>> {
//     fn new(data: T) -> Self {
//         NonEmpty(vec![data])
//     }
// }

// fn main() {
//     let non_empty_vec = NonEmpty::new(10);
//     println!("{:?}", non_empty_vec);
// }

// //Namespace Organization: Unit structs can be used to organize related functions or methods into a single namespace. By defining them within a unit struct, you can group related functionality together and access them using the :: syntax.
// mod math {
//     pub struct Operations;

//     impl Operations {
//         pub fn add(a: i32, b: i32) -> i32 {
//             a + b
//         }

//         pub fn multiply(a: i32, b: i32) -> i32 {
//             a * b
//         }
//     }
// }

// fn main() {
//     let result = math::Operations::add(2, 3);
//     println!("Addition: {}", result);
// }

// // Code Documentation: Unit structs can be used to document certain concepts or intentions in your code. By giving a meaningful name to a unit struct, you can convey the purpose or intention of a particular code block or construct.
// mod logging {
//     pub struct Log;

//     impl Log {
//         pub fn info(message: &str) {
//             println!("INFO: {}", message);
//         }

//         pub fn error(message: &str) {
//             println!("ERROR: {}", message);
//         }
//     }
// }

// fn main() {
//     logging::Log::info("This is an informational message");
//     logging::Log::error("An error occurred!");
// }

// // Future Extensibility: Unit structs can be used as placeholders for potential future enhancements or extensions. You can define a unit struct with the intention of adding more fields or functionality to it in future versions of your code.
// struct FutureExtension;

// impl FutureExtension {
//     fn new() -> Self {
//         FutureExtension
//     }

//     // Additional methods or fields can be added in the future
//     fn additional_feature(&self) {
//         println!("Performing additional feature...");
//     }
// }

// fn main() {
//     let future = FutureExtension::new();
//     future.additional_feature();
// }
*/

// -------------------------------------------
// 			- Zero Sized Types
// 		        - Phantom Data
// -------------------------------------------

// struct ABC;
// use negative_impl::negative_impl;

// #[negative_impl]
// impl !Send for ABC {}

// #[negative_impl]
// impl !Sync for ABC {}

use std::{marker::PhantomData, mem::size_of, rc::Rc};
struct ABC {
    ensuring_no_send_sync: PhantomData<Rc<()>>,
}
fn main() {
    println!("{}", size_of::<ABC>());
}
