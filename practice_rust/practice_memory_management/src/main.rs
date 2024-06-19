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
