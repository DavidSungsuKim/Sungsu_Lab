fn main() {
    println!("Hello, world!");

    // shadow
    let t = 10;
    let t = 1.2;
    println!("t = {t}");

    // constants
    const MAX_VAL: i32 = 100;

    // Type aliasing
    type Age = u8;
    let peter_age: Age = 42;

    // Type conversion
    let a = 10;
    let b = a as f64;
}
