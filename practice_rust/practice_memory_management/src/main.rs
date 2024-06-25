/*
// -------------------------------------------
//         - Concrete Lifetimes
// -------------------------------------------

fn main() {
    // Example 1:
    let i = 5;
    let j = i;
    println!("{i}");

    // Example 2:
    let str_1 = String::from("abc");
    let str_2 = str_1;
    //println!("str_1: {str_1}");

    // Example 3:
    let str_1 = String::from("abc");
    str_fn(str_1);
    //let str_2 = str_1;

    // Example 4:
    let i;
    {
        let j = 5;
        i = &j;
        println!("i: {i}");
    }

    // Example 5:
    let mut vec_1 = vec![6, 5, 8, 9];
    let ref_1 = &vec_1;
    println!("ref 1: {:?}", ref_1);
    let ref_2 = &mut vec_1;
    ref_2.push(3);
    println!("ref 2: {:?}", ref_2);
}

fn str_fn(s: String) {
    println!("s: {s}");
}
*/

// -------------------------------------------
// 			Generic Lifetimes
// -------------------------------------------

// Example 1:
/* fn main() {
    let int1 = 5;
    let int2 = 10;
    let picked_value = picking_int(&int1, &int2);
    println!("{picked_value}");
}

fn picking_int<'a>(i: &'a i32, j: &'a i32) -> &'a i32 {
    if rand::random() {
        i
    } else {
        j
    }
}
*/

// Example 2:
/*
fn main(){
let int1 = 5;
    {
        let int2 = 10;
        let picked_value = picking_int(&int1, &int2);
        println!("{picked_value}");
    }
}

fn picking_int<'a>(i: &'a i32, j: &'a i32) -> &'a i32 {
    if rand::random() {
        i
    } else {
        j
    }
}
*/

// Example 3:
/* fn main() {
    let int1 = 5;
    let picked_value;
    {
        let int2 = 10;
        picked_value = picking_int(&int1, &int2);
    }
    //println!("{picked_value}");
}

fn picking_int<'a>(i: &'a i32, j: &'a i32) -> &'a i32 {
    if rand::random() {
        i
    } else {
        j
    }
}
*/

// Example 4:
/*
fn main() {
    let int1 = 5;
    let picked_value;
    {
        let int2 = 10;
        picked_value = picking_int(&int1, &int2);
    }
    println!("{picked_value}");
}

fn picking_int(i: &i32, j: &i32) -> &'static i32 {
    let y: &'static i32 = &6;
    y
}
*/

/*
// Problem 1: Fix the code by moving the indicated line to approperiate location

fn main() {
    let mut some_str = String::from("I am String");
    let ref1 = &some_str;
    println!("{ref1}"); // move this line only

    let ref2 = &mut some_str;
    ref2.push_str(" additional information");
    println!("{ref2}");
}
*/

/*
// Problem 2: Fix the code by moving the indicated line to approperiate location

fn identity(a: &i32) -> &i32 {
    a
}

fn main() {
    let mut x_ref: Option<&i32> = None;
    {
        let x = 7;
        x_ref = Some(identity(&x));
        assert_eq!(*x_ref.unwrap(), 7); // Issue at this line
    }
}
*/

/*
//Problem 3: Fix the code by moving the indicated line to approperiate place

fn option(opt: Option<&i32>) -> &i32 {
    opt.unwrap()
}
fn main() {
    let y = 4; // move this line only
    let answer = { option(Some(&y)) };

    assert_eq!(answer, &4);
}
*/

/*
// Problem 4: Add lifetime annotations in the function signature

fn some_if_greater<'a>(number: &'a i32, greater_than: &'a i32) -> Option<&'a i32> {
    if number > greater_than {
        Some(number)
    } else {
        None
    }
}
fn main() {
    let num_1 = 7;
    let greater_val = 4;
    let test = some_if_greater(&num_1, &greater_val);
}
*/

// -------------------------------------------
// 	        Lifetime Elision
// -------------------------------------------

/*
1. Each parameter that is a reference, gets its own lifetime parameter.
2. If there is exactly one input lifetime parameter, that lifetime is assigned to
    all output lifetime parameters.
3. If there are multiple input lifetime parameters, but one of them is &self or &mut self,
   the lifetime of self is assigned to all output lifetime parameters.
*/

/*
//Example 1:
fn main() {
    let str_1 = "some str";

    let recieved_str = return_str(&str_1);
}
// Code with Lifetime Elision
// fn return_str(s_1: &str) -> &str {
//     s_1
// }

// Code without Lifetime Elision
fn return_str<'a>(s_1: &'a str) -> &'a str {
    s_1
}
*/

// Example 2:
/* fn main() {
    let str_1 = "some str";
    let str_2 = "other str";
    let recieved_str = return_str(&str_1, &str_2);
}

fn return_str<'a, 'b>(s_1: &'a str, s_2: &'b str) -> &'a str {
    s_1
}
*/

/*
// Problem 1: The code is compiling. However, I want you to remove the
// extra lifetimes parameters from function signatures.

struct DataHolder<'a> {
    data: Vec<&'a str>,
}

impl<'a> DataHolder<'a> {
    fn initialize() -> Self {
        DataHolder { data: Vec::new() }
    }

    fn add_data(&mut self, item: &'a str) {
        self.data.push(item);
    }

    fn extract_data_containing_substring<'b>(&mut self, sub: &'b str) -> &'a str {
        // --> here 'b is not necessary since the return type is already supposed to be 'a.
        for i in 0..self.data.len() {
            if self.data[i].contains(sub) {
                return self.data.remove(i);
            }
        }
        panic!("Data containing substring not found");
    }
}

fn main() {
    let mut my_data = DataHolder::initialize();
    my_data.add_data("Apple");
    my_data.add_data("Banana");
    my_data.add_data("Cherry");
    my_data.add_data("Date");
    let extracted = my_data.extract_data_containing_substring("na");
    println!("Extracted: {}", extracted);
    assert_eq!(my_data.data.len(), 3);
}
*/

/*
// Problem 2: Identify the function signature which needs explicity lifetime parameters.
// For those, which do not require explicit lifetimes parameters, write their expanded code which will be generated by the compiler.

// Do not compile. This is not a program

fn print(s: &str) {}                                      // does not need explicit lifetime
fn print<'a>(s: &'a str) {}                               // expands code by compiler

fn debug(v: usize, s: &str) {}                            // does not need explicit lifetime
fn debug<'a>(v: usize, s: &'a str) {}                     // expands code by compiler

fn substr(s: &str, until: usize) -> &str {}               // does not need explicit lifetime
fn substr<'a>(s: &'a str, until: usize) -> &'a str;       // expands code by compiler

fn get_str() -> &str {}                                    // needs explicit lifetimes

fn frob(s: &str, t: &str) -> &str{}                        // needs explicit lifetimes

fn get_mut(&mut self) -> &mut T; 	                   // does not need explicit lifetime
fn get_mut<'a>(&'a mut self) -> &'a mut T;	           // expands code by compiler

fn new(buf: &mut [u8]) -> BufWriter;                       // does not need explicit lifetime
fn new<'a>(buf: &'a mut [u8]) -> BufWriter<'a>;            // expanded
*/

/*
// -------------------------------------------
// 			Lifetimes in Structs
// -------------------------------------------

// 1. Each paramter that is a reference, gets its own lifetime parameter.
// 2. If there is exactly one input lifetime parameter, that lifetime is assigned to all output lifetime parameters.
// 3. If there are multiple input lifetime parameters, but one of them is &self or &mut self,
//    the lifetime of self is assigned to all output lifetime parameters.

struct ArrayProcessor<'a> {
    data: &'a [i32],
}
impl<'a> ArrayProcessor<'a> {
    fn update_data(&mut self, new_data: &'a [i32]) -> &[i32] {
        //fn update_data<'b>(&'b mut self, new_data: &'a [i32]) -> &'b [i32] { // expanded by compiler
        let previous_data = self.data;
        self.data = new_data;
        previous_data
    }
}
fn main() {
    let mut some_data = ArrayProcessor { data: &[4, 5, 6] };

    let previous_data = some_data.update_data(&[5, 8, 10]);
    println!("Previous data: {:?}", previous_data);
    println!("New data: {:?}", some_data.data);
}

*/

/*
// -------------------------------------------
//         Box Smart Pointer (Part 1)
// -------------------------------------------

//       Simple Pointer          ||         Smart Pointers
// ----------------------------------------------------------------------
// Just stores memory address    ||   Special capabilities
// Indicated by &                ||   Not just simple references
// Also called references        ||
// No special capabilities       ||

// enum Conveyance {
//     Car(i32),
//     Train(i32),
//     Air(i32),
//     Walk
}

#[derive(Debug)]
enum List {
    Cons(i32, Box<List>),
    Nil,
}

fn main() {
    // let x = 0.625;
    // let y = Box::new(x);
    // let z = &x;

    let list = List::Cons(
        1,
        Box::new(List::Cons(2, Box::new(List::Cons(3, Box::new(List::Nil))))),
    );

    println!("{:?}", list);
}
*/

// -------------------------------------------
//         Box Smart Pointer (Part 2)
// -------------------------------------------

// Example 1
/*
#[derive(Debug)]
enum List {
    Cons(i32, Option<Box<List>>),
}

fn main() {
    let list = List::Cons(
        1,
        Some(Box::new(List::Cons(2, Some(Box::new(List::Cons(3, None)))))),
    );

    println!("{:?}", list);
}
*/

/*
// Example 2
struct Huge_Data;
struct Small_Data;

trait Storage {}

impl Storage for Huge_Data {}
impl Storage for Small_Data {}

fn main() {
    let data_1 = Huge_Data;
    let data_2 = Box::new(Huge_Data);

    let data_3 = data_1;
    let data_4 = data_2;

    let data_5 = Box::new(Small_Data);

    let data: Vec<Box<dyn Storage>> = vec![Box::new(data_3), data_4, data_5];
}
*/

/*
// Problem 1: Fix the code below so that it compiles

enum BinaryTree {
    Leaf,
    Node(i32, Box<BinaryTree>, Box<BinaryTree>),
}

fn main() {}
*/

/*
// Problem 2: Fix the code by completing the function signature

struct Wrapper {
    data: String,
}

fn modify_data(mut wrapper: Box<Wrapper>) -> Box<Wrapper> {
    wrapper.data = String::from("Modified");
    wrapper
}

fn main() {
    let original_wrapper = Box::new(Wrapper {
        data: String::from("Original"),
    });
    let modified_wrapper = modify_data(original_wrapper);
}
*/

/*
// Problem 3: Complete the code below
// Solution:

#[derive(Debug)]
enum ListNode<T> {
    Node(T, Box<ListNode<T>>),
    None,
}

fn main() {
    // Create a linked list representing: Node(1, Node(2, Node(3, Node(4, None))))
    let list = ListNode::Node(
        1,
        Box::new(ListNode::Node(
            2,
            Box::new(ListNode::Node(
                3,
                Box::new(ListNode::Node(4, Box::new(ListNode::None))),
            )),
        )),
    );
    println!("{:?}", list);
}
*/

/*
// Problem 4: Fix the code by adding the type annotation

struct AudioSample;
struct ImageFile;

trait Media {}

impl Media for AudioSample {}
impl Media for ImageFile {}

fn main() {
    let audio_1 = AudioSample;
    let audio_2 = Box::new(AudioSample);

    let audio_3 = audio_1;
    let audio_4 = audio_2;

    let image_1 = Box::new(ImageFile);

    let media_collection: Vec<Box<dyn Media>> = vec![Box::new(audio_3), audio_4, image_1];
}
*/

/*
// -------------------------------------------
// 		Reference Counting Smart Pointer
// -------------------------------------------

use std::rc::Rc;
enum List {
    Cons(i32, Option<Rc<List>>),
}
fn main() {
    let a = Rc::new(List::Cons(1, Some(Rc::new(List::Cons(2, None)))));
    println!("Reference count after a: {}", Rc::strong_count(&a));
    {
        let b = List::Cons(3, Some(Rc::clone(&a)));
        println!("Reference count after b: {}", Rc::strong_count(&a));

        let c = List::Cons(4, Some(Rc::clone(&a)));
        println!("Reference count after c: {}", Rc::strong_count(&a));
    }
    println!("Reference count after scope: {}", Rc::strong_count(&a));
}
*/

/*
// Problem 1: Put correct values in the assert_eq! macro so that the code does not panic
// Only use numbers as second parameter to assert_eq's.

use std::rc::Rc;
#[derive(Debug)]
struct ListNode<T> {
    value: T,
    next: Option<Rc<ListNode<T>>>,
}

fn main() {
    let node_3 = Rc::new(ListNode {
        value: 3,
        next: None,
    });

    let node_2 = Rc::new(ListNode {
        value: 2,
        next: Some(Rc::clone(&node_3)),
    });

    let node_1 = Rc::new(ListNode {
        value: 1,
        next: Some(Rc::clone(&node_2)),
    });

    assert_eq!(Rc::strong_count(&node_1), 1); // put a value inplace of ?
    assert_eq!(Rc::strong_count(&node_2), 2); // put a value inplace of ?
    assert_eq!(Rc::strong_count(&node_3), 3); // put a value inplace of ?
}
*/

/*
// Problem: In the code below, we would like to code the idea that multiple
// users can own a file. Complete the code by adding relevant code

use std::rc::Rc;

struct File {}

struct User {
    file: Rc<File>, /*You code here*/
}

fn main() {
    let txt_file = Rc::new(File {});

    let user_1 = User {
        file: Rc::clone(&txt_file), /*You code here*/
    };

    let user_2 = User {
        file: Rc::clone(&txt_file), /*You code here*/
    };
}
*/

/*
// -------------------------------------------
// 		RefCell Smart Pointer
// -------------------------------------------

use std::{cell::RefCell, rc::Rc};

fn main() {
    // Example 1: Borrowing rules at run time
    // let mut x = 50;
    // let x1 = &x;
    // let x2 = &x;
    // let x3 = &mut x;

    // println!("{} {} ", x1, x2);

    let a = RefCell::new(10);

    //{ // add the scope later on
    let b = a.borrow();
    let c = a.borrow();
    //}

    drop(b); // add later on, remove after adding the scope above
    drop(c); // add later on
    let d = a.borrow_mut();
    // drop(d) // add later on
    //println!("{} {}", b, c); // later on delete this
    //println!("Value of a is : {:?}", a); // add later on

    // Example 2: Interior mutability
    // let x = 32;
    // let x1 = &mut x;

    let a = RefCell::new(10);
    //let c = *a; // add later on
    let mut b = a.borrow_mut();
    *b = 15;

    drop(b); // add later on
    println!("{:?}", a);

    // Example 3
    let a = Rc::new(RefCell::new(String::from("c++")));
    let b = Rc::clone(&a);

    *b.borrow_mut() = String::from("rust");
    println!("{:?}", a);
}
*/

/*
// Problem 1: fill in the code for TODO's

use std::cell::RefCell;
fn main() {
    let data: RefCell<Option<i32>> = RefCell::new(Some(42));

    // TODO: Use borrow_mut to safely modify the value inside the RefCell to None.
    let mut p = data.borrow_mut();
    *p = None;
    drop(p);

    if data.borrow().is_some() {
        println!("Final value: {:?}", data.borrow());
    } else {
        println!("No value present.");
    }
}
*/

/*
// Problem 2: Fix the lines indicated in the code so that it compiles

use std::cell::RefCell;
struct Car {
    model: String,
    price: u32,
    status: RefCell<&'static str>,
}

impl Car {
    fn new(model: &str, price: u32) -> Self {
        Car {
            model: model.to_owned(),
            price,
            status: RefCell::new("Available"),
        }
    }

    fn sold(&self) {
        let new_status = match self.price {
            0..=50000 => "Sold - Economy",
            50001..=100000 => "Sold - Mid Range",
            _ => "Sold - Luxury",
        };
        *self.status.borrow_mut() = new_status;
    }
}

fn main() {
    let car = Car::new("Sedan", 75000);
    car.sold();
    println!("Car Status: {}", car.status.borrow()); // Fix this line
}
*/

/*
// Problem 3: The code below will compile. However it will panic at execution time.
// Task 1: Your first ttask is to add some code so that the it does not panice at execution time
// Task 2: The value at the last print line will not be displayed.
// Instead of value, <Borrowed> will be displayed.
// Add appropriate code so that the value of x is being displayed.

use std::cell::RefCell;

fn main() {
    let x = RefCell::new(5);
    let x_ref1 = x.borrow();
    let x_ref2 = x.borrow();
    println!("x_ref1: {}, x_ref2: {}", x_ref1, x_ref2);

    /* Code for Task 1 */
    drop(x_ref1);
    drop(x_ref2);

    let mut x_ref3 = x.borrow_mut();
    *x_ref3 = 6;

    /* Code for Task 2 */
    drop(x_ref3);

    println!("Stored value: {:?}", x);
}
*/

// -------------------------------------------
// 		RefCell Example
// -------------------------------------------
use std::{cell::RefCell, rc::Rc};

#[derive(Debug)]
struct File {
    active_user: u32,
}

#[derive(Debug)]
struct User {
    file: Rc<RefCell<File>>,
}

fn main() {
    let mut txt_file = Rc::new(RefCell::new((File { active_user: 0 })));

    let mut user_1 = User {
        file: Rc::clone(&txt_file),
    };
    user_1.file.borrow_mut().active_user += 1;
    println!("Active users: {:?}", txt_file.borrow().active_user);

    let mut user_2 = User {
        file: Rc::clone(&txt_file),
    };
    user_2.file.borrow_mut().active_user += 1;
    println!("Active users: {:?}", txt_file.borrow().active_user);
}
