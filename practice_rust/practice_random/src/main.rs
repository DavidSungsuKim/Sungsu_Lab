
fn main() 
{
    func_hello_world();
    
    let mut x = func_lambda();
    x += 1;

    let y = func_show_argument(x);

    let mut z: u32 = 1;
    func_show_ref_argument(&mut z);
    println!("z={}",z);

    func_show_data_types();
    func_show_if_expressions();
    func_show_loops();
    func_show_ref();
    func_show_slice_type();
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
    let x = 2.1;
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
    while( count != 0 )
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

    let mut s = String::from("hello");
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
}
     */
}

fn func_show_slice_type()
{

}