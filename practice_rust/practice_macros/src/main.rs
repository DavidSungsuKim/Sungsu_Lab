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

macro_rules! string_concat {
    /*
    () => {
        String::new();
    };

    ($some_str: expr) => {{
        let mut temp_str = String::new();
        temp_str.push_str($some_str);
        temp_str
    }
    };

    ($some_s1: expr, $some_s2:expr) => {{
        let mut temp_str = String::new();
        temp_str.push_str($some_s1);
        temp_str.push_str($some_s2);

        temp_str
    }
};
*/

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
