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

    /* Compound Data Types */
    // Strings
    let fixed_string = "Fixed string";
    let mut flexible_string = String::from("This string will grow");
    flexible_string.push('s');

    // Arrays
    let mut array_1 = [4, 5, 6, 7, 8];
    let num = array_1[3];
    println!("{:?}", array_1);

    // Vectors
    let vec_1: Vec<i32> = vec![4, 5, 6, 8, 9];
    let num = vec_1[3];

    let my_info = ("Salary", 4000, "Age", 40);
    let Salary_value = my_info.1;
    let (salary, salary_value, age, age_value) = my_info;
}
