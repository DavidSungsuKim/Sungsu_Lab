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

    /* Functions and Code blocks */
    my_fn("This is my func");
    let str = "Function call with a variable";
    my_fn(str);
    //  my_fn(String::from("str")); // This doesn't compile.

    let answer = multiplication(1, 2);
    let (mul, add, sub) = basic_math(1, 2);

    let full_name = {
        let first = "Othani";
        let last = "Shohei";
        format!("{first} {last}")
    };

    println!("{full_name}");

    /* Conditionals */
    // If-else
    let num = 40;
    if num < 50 {
        println!("The number is less than 50");
    } else {
        println!("The number is greater than or equal to 50");
    }

    // If else if ladder
    let marks = 95;
    let mut grade = 'N';

    if marks >= 90 {
        grade = 'A';
    } else {
        grade = 'F';
    }

    let grade = if marks >= 90 {
        grade = 'A';
    } else {
        grade = 'F';
    };

    let grade = if marks >= 90 { 'A' } else { 'F' };

    // Match
    let grade;
    match marks {
        90..=100 => grade = 'A',
        _ => grade = 'F',
    }

    let grade = match marks {
        90..=100 => 'A',
        _ => 'F',
    };
    println!("Grade={grade}");

    /* Control Flow */
    // loops
    loop {
        println!("simple loop!");
        break;
    }

    let mut count = 0;
    'outer: loop {
        loop {
            break 'outer;
        }
        count = count + 1;
    }
    println!("loop count={count}");

    let a = loop {
        break 5;
    };
    println!("a={a}");

    // for loops
    let vec = vec![1, 2, 3, 4, 5];
    for i in vec {
        println!("{i}");
    }

    // While loops
    let mut num = 0;
    while num < 10 {
        num = num + 1;
    }
}

fn my_fn(s: &str) {
    println!("{s}");
}

fn multiplication(num1: i32, num2: i32) -> i32 {
    num1 * num2
}

fn basic_math(num1: i32, num2: i32) -> (i32, i32, i32) {
    (num1 * num2, num1 + num2, num1 - num2) // brackets are mandatory!
}
