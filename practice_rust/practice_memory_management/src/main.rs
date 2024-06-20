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

// -------------------------------------------
// 			Lifetimes in Structs
// -------------------------------------------

/*
1. Each paramter that is a reference, gets its own lifetime parameter.
2. If there is exactly one input lifetime parameter, that lifetime is assigned to all output lifetime parameters.
3. If there are multiple input lifetime parameters, but one of them is &self or &mut self,
   the lifetime of self is assigned to all output lifetime parameters.
*/
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
