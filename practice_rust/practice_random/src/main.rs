
use clap::{Arg, Command, ArgAction};

fn main() 
{
    func_hello_world();
    
    let mut x = func_lambda();
    x += 1;

    let y = func_show_argument(x);

    let mut z: u32 = 1;
    func_show_ref_argument(&mut z);
    println!("z={}",z);

    println!("func_show_data_types");
    func_show_data_types();
    println!("func_show_if_expressions"); 
    func_show_if_expressions();
    println!("func_show_loops"); 
    func_show_loops();
    println!("func_show_ref"); 
    func_show_ref();
    println!("func_show_slice_type"); 
    func_show_slice_type();
    println!("func_show_a_struct"); 
    func_show_a_struct();
    println!("func_show_method"); 
    func_show_method();
    println!("func_show_enum_example"); 
    func_show_enum_example();
    println!("func_show_match_enum_example"); 
    func_show_match_enum_example();
    println!("func_show_vector_example"); 
    func_show_vector_example(); 

    func_test_clap_parser();
}

fn func_test_clap_parser()
{
    let matches = Command::new("MyApp")
    .arg(Arg::new("out")
        .long("output")
        .required(true)
        .action(ArgAction::Set)
        .default_value("-"))
    .arg(Arg::new("cfg")
        .short('c')
        .action(ArgAction::Set))
    .get_matches(); // builds the instance of ArgMatches

    if let Some(output) = matches.get_one::<String>("out") {
        println!("Value for --output: {output}");
    }

    // to get information about the "cfg" argument we created, such as the value supplied we use
    // various ArgMatches methods, such as [ArgMatches::get_one]
    if let Some(c) = matches.get_one::<String>("cfg") {
        println!("Value for -c: {c}");
    }    
}

fn func_hello_world()
{
    println!("hello world!");
}

fn func_show_argument(a: u32) -> u32
{
    return a;
}

fn func_show_ref_argument(a: &mut u32)
{
    *a += 1;
}

fn func_lambda() -> u32 { 1 }

fn func_show_data_types()
{
    // bool types
    let t = true;
    let f: bool = false;

    // integer types
    let i_8: i8 = 1;
    let i_128: i128 = 1;
    let i_arch: isize = 1;
    let u_arch: usize = 1;

    let u_decimal = 12_345; // no didfference with 12345, 123_45, and etc
    let u_hex = 0xff;
    let u_octal = 0o77;
    let u_binarh = 0b1111_0000;
    let u_byte = b'A';
    println!("u_decimal={}", u_decimal);
    println!("u_hex={}", u_hex);
    println!("u_octal={}", u_octal);
    println!("u_binarh={}", u_binarh);
    println!("u_byte={}", u_byte);

    // floating-point type
    let x: f64 = 2.1;
    let y: f32 = 2.1;
    println!("x={}",x);
    println!("y={}",y);

    // character types
    let c = 'a';
    let z: char = 'Z';
    let heart_eyed_cat = '😻';
    println!("heart_eyed_cat={}", heart_eyed_cat);

    // tuple type
    let tup = (100, 1.0, 1);
    let (tup_x, tup_y, tup_z) = tup;
    println!("tup_x={}, tup_y={}, tup_z={}", tup_x, tup_y, tup_z);

    // array type
    let array = [1,2,3,4,5];
    let array_names = ["matthew", "mark", "luke", "john"];

}

fn func_show_if_expressions()
{
    let num = 1;
    if num > 1
    {
        println!("true");
    }
    else 
    {
        println!("false");        
    }

    let statement = if num > 1 { true } else { false };
    println!("statement={}", statement);
}

fn func_show_loops()
{
    let mut i = 0;
    loop 
    {
        println!("i={}", i);
        i += 1;

        if i == 10
         {
            break;
         }
    }

    let mut count = 10;
    while count != 0 
    {
        println!("count={}", count);
        count -= 1;
    }

    let array = [1,2,3,4,5];
    for a in array
    {
        println!("a={}", a);
    }

}

fn func_show_ref()
{
    let a = 1;
    let ref_a = &a;
    println!("ref_a={}", ref_a);

    let mut b = 2;
    let ref_b = &mut b; // even if b is known to be 'mut', mut must be specified again here.
    println!("ref_b={}", ref_b);

    *ref_b = 3;  
    println!("ref_b={}", ref_b);

    let mut s: String = String::from("hello");
    let ref_s = &mut s;
    let ref_s = "hallo";
    println!("ref_s={}", ref_s);

    /* NOTE:
     * This case isn't allowed in Rust: referencing as immuatable and mutable at the same time,
     * which is one of the important features in Rust
     {
        let mut s = String::from("hello");
        let r1 = &s; // no problem
        let r2 = &mut s; // BIG PROBLEM
    
        println!("{} and {}", r1, r2);
    }    
    */

    /* NOTE:
     * Dangling pointer are also inherently not allowed in Rust like this one:
    fn dangle() -> &String 
    {
        let s = String::from("hello");
        &s
    }
    */
}

fn func_show_slice_type()
{
    let mut s = String::from("hello world");
    let word  = first_word( &s );

    println!("word={}", word);
    let byte = s.as_bytes()[word];
    println!("s[{}]={}", word, byte );

    s.clear();

    let ss = String::from("hello world");
    let hello = &ss[0..5];
    let world = &ss[6..11];
    println!("{} {}", hello, world);

    /* This causes a panic since index 12 is out of bound.
    let world = &ss[6..12]; 
    */
}

fn first_word(s: &String) -> usize 
{
    let bytes = s.as_bytes();

    for (i, &item) in bytes.iter().enumerate() 
    {
        if item == b'w' 
        {
            return i;
        }
    }

    s.len()
}

struct User
{
    active: bool,
    username: String,
    email: String,
    sign_in_count: u64,
}

fn func_show_a_struct()
{
    let user1 = User{ active: true, 
                            username: String::from("JohnDoe"), 
                            email: String::from("me@com.com"), 
                            sign_in_count: 1 };

    let user2 = User{ active: user1.active, 
                            username: user1.username,
                            email: user1.email,
                            sign_in_count: user1.sign_in_count };

    struct Color(i32, i32, i32);
    let black = Color(0,0,0);
    
}

fn build_user( email: String, username: String) -> User 
{
    User 
    {
        active: true,
        username,
        email,
        sign_in_count:1
    }
}

struct Rectangle 
{
    width: u32,
    height: u32
}

impl Rectangle 
{
    fn area(&self) -> u32
    {
        self.width * self.height
    }

    fn half_area(&self) -> u32
    {
        ( self.area() / (2 as u32) ) as u32
    }
}

impl Rectangle
{
    fn aux( &self ) -> bool
    {
        true
    }
}

fn func_show_method( )
{
    let rec1 = Rectangle
    {
        width: 10,
        height: 5
    };

    println!( "height={}, width={}", rec1.height, rec1.width );
    println!( "area={}", rec1.area() );
    println!( "half area={}", rec1.half_area() );
    println!( "aux={}", rec1.aux() );
}

enum IpAddr 
{
    V4(u8, u8, u8, u8),
    V6(String),
}

enum Message
{
    Quit,
    Move { x: i32, y: i32 },
    Write ( String ),
    ChangeColor( i32, i32, i32 ),
}

enum Option<T>
{
    None,
    Some(T),
}

fn func_show_enum_example()
{
    let home = IpAddr::V4(127, 0, 0, 1);
    let msg = Message::Move { x: 1, y: 2 };
    let optNone: Option<u32> = Option::None;
    let optSome1 = Some(1);
    let optSome2 = Some(5);
    let optSome3 = Some("Hello");    
}

fn func_show_match_enum_example()
{
    let dice_roll = 9;
    match dice_roll 
    {
        3 => println!("3"),
        7 => println!("7"),
        _other => println!("other"),    // _other can be replaced with '_'
    }

    //let m = Message::Quit;
    //let m = Message::Move{ x:1, y:2 };
    //let m = Message::Write(String::from("Hello"));
    let m = Message::ChangeColor(1,2,3);

    match m
    {
        Message::Quit => println!("Quit"),
        Message::Move{ x, y } => println!("x={}, y={}", x, y),
        Message::Write(string) => println!("string={}", string),
        Message::ChangeColor( a, b, c) => println!("a={}, b={}, c={}", a,b,c)
    }
}

fn func_show_vector_example()
{
    let v: Vec<i32> = Vec::new();

    let v2 = vec![1,2,3];

    let mut v3: Vec<i32> = Vec::new();
    v3.push(1);
    v3.push(2);

    let v3First : &i32 = &v3[0];

    let v3Third : std::option::Option<&i32> = v3.get(0);
    match v3Third
    {
        Some(v3Third) => println!("some"),
        None => println!("none"),
    }
}