/* Ownership basics */

/* Note that these don't pertain to primitive types but to types like String, Vector, and other user-defined types
1. Each value has a variable that's its owner.
2. A value can have only one owner at a time.
3. If the owner goes out of scope, the value is cleaned up

==> more memory safe, no run-time overhead
*/

/*
- Borrrowing Rules
    - At any time, you can have either one mutable reference or any number of immutable references.
    - References must always be valid.
- Solve out two problems
    - Data race
    - Dangling references
*/

fn main() {
    let vec_1 = vec![1, 2, 3];

    take_ownership(vec_1);
    //println!("vec_1: {:?}", vec_1);  // This is not allowed since it has gone for another ownwer. Do vec_1.clone() instead.

    let vec_2 = give_ownership();
    println!("vec_2: {:?}", vec_2);

    let vec_3 = take_and_give_ownership(vec_2); // vec_2 works too instead of vec_3 as Rust allows to define the same variable name again.
    println!("vec_3: {:?}", vec_3);

    let x = 10;
    stack_function(x);
    println!("x: {x}");

    /* borrowing */
    let mut vec_1 = vec![4, 5, 6];
    let ref1 = &vec_1;
    let ref2 = &vec_1;
    println!("ref1: {:?}, ref2: {:?}", ref1, ref2);
    let ref3 = &mut vec_1;

    let mut vec_1 = vec![1, 2, 3];
    let ref1 = &vec_1;
    borrows_vec(ref1);
    let ref2 = &mut vec_1;
    mutably_borrows_vec(ref2);
    println!("vec 1 is: {:?}", vec_1);

    /* Dereferencing */
    let mut some_data = 42;
    let ref_1 = &mut some_data;
    let deref_copy = *ref_1;
    *ref_1 = 13;
    println!("some_data is: {some_data}, deref_copy is: {deref_copy}");

    let mut heap_data = vec![5, 6, 7];
    let ref_1 = &heap_data;
    let ref_2 = ref_1;
    let ref_3 = ref_1;
    let deref_copy = ref_1.clone();

    let move_out = ref_1;
    // let move_out_again = ref_1;
}

fn borrows_vec(vec: &Vec<i32>) {
    println!("vec is: {:?}", vec);
}

fn mutably_borrows_vec(vec: &mut Vec<i32>) {
    vec.push(10);
}

fn gives_onwership() -> Vec<i32> {
    vec![4, 5, 6]
}

fn take_ownership(vec: Vec<i32>) {
    println!("vec: {:?}", vec);
}

fn give_ownership() -> Vec<i32> {
    vec![4, 5, 6]
}

fn take_and_give_ownership(mut vec: Vec<i32>) -> Vec<i32> {
    vec.push(10);
    vec
}

fn stack_function(mut var: i32) {
    // a copy is made to var from original variable
    var = 56;
    println!("In func, var: {var}");
}
