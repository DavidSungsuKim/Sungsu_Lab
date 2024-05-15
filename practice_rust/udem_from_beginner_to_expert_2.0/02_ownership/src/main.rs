/* Ownership basics */

/* Note that these don't pertain to primitive types but to types like String, Vector, and other user-defined types
1. Each value has a variable that's its owner.
2. A value can have only one owner at a time.
3. If the owner goes out of scope, the value is cleaned up

==> more memory safe, no run-time overhead
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
