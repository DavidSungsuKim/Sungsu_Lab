/*
// This doesn't compile due to use rust_course::Student;
// -------------------------------------------
// 		Initializing Struct Instance
// -------------------------------------------

use rust_course::Student;
fn main() {
    let std_1 = Student::new("joseph".to_string()).unwrap_or_default();
    println!("{:?}", std_1);

    let std_2 = Student::default();
    println!("{:?}", std_2);

    let std_3 = Student {
        age: 12,
        ..Default::default()
    };
}

// code in lib

#[derive(Debug, Default)]
pub struct Student {
    id: u8, // add later on
    pub age: u8,
    pub name: String,
}

impl Student {
    // pub fn new(std_name: String) -> Self {
    //     Self {
    //         id: 0,
    //         age: 20,
    //         name: std_name,
    //     }
    // }

    pub fn new(std_name: String) -> Result<Self, String> {
        let x = std_name.chars();
        if std_name.chars().all(|x| matches!(x, 'a'..='z')) {
            Ok(Self {
                id: 0,
                age: 20,
                name: std_name,
            })
        } else {
            Err("The name is invalid".to_string())
        }
    }
}

// impl Default for Student {
//     fn default() -> Self {
//         Self {
//             id: 0,
//             name: "unknown".to_string(),
//             age: 20,
//         }
//     }
// }
*/

/*
// -------------------------------------------
// 			Builder Pattern
// -------------------------------------------

#[derive(Debug, Default, Clone)]
struct Customer {
    name: String,
    username: String,
    membership: Membershiptype,
    gender: char,
    country: String,
    age: u8,
}

#[derive(Debug, Clone)]
enum Membershiptype {
    new,
    causual,
    loyal,
}

impl Default for Membershiptype {
    fn default() -> Self {
        Membershiptype::new
    }
}

impl Customer {
    fn new(name: String) -> CustomerBuilder {
        CustomerBuilder {
            name: name,
            ..Default::default() // username: None,
                                 // membership: None,
                                 // gender: None,
                                 // country: None,
                                 // age: None,
        }
    }
    // fn new(name: String) -> Self {
    //     Customer {
    //         name: name,
    //         ..Default::default()
    //     }
    // }

    // fn new_2(name: String, username: String) -> Self {
    //     Customer {
    //         name: name,
    //         username: username,
    //         ..Default::default()
    //     }
    // }

    // fn new_3(name: String, username: String, membership: Membershiptype) -> Self {
    //     Customer {
    //         name: name,
    //         username: username,
    //         membership: membership,
    //         ..Default::default()
    //     }
    // }
}

#[derive(Default)]
struct CustomerBuilder {
    name: String,
    username: Option<String>,
    membership: Option<Membershiptype>,
    gender: Option<char>,
    country: Option<String>,
    age: Option<u8>,
}

impl CustomerBuilder {
    fn username(&mut self, username: String) -> &mut Self {
        self.username = Some(username);
        self
    }

    fn membership(&mut self, membership: Membershiptype) -> &mut Self {
        self.membership = Some(membership);
        self
    }

    fn gender(&mut self, gender: char) -> &mut Self {
        self.gender = Some(gender);
        self
    }
    fn country(&mut self, country: String) -> &mut Self {
        self.country = Some(country);
        self
    }
    fn age(&mut self, age: u8) -> &mut Self {
        self.age = Some(age);
        self
    }
    fn build(&mut self) -> Customer {
        Customer {
            name: self.name.clone(),
            username: self.username.clone().unwrap_or_default(),
            membership: self.membership.clone().unwrap_or_default(),
            gender: self.gender.unwrap_or_default(),
            country: self.country.clone().unwrap_or_default(),
            age: self.age.unwrap_or_default(),
        }
    }
}
fn main() {
    // let new_user = Customer::new("Nouman".to_string());
    // let user_with_login = Customer::new_2("Joseph".to_string(), "joe123".to_string());
    // let user_with_membership = Customer::new_3(
    //     "Micheal".to_string(),
    //     "micheal2000".to_string(),
    //     Membershiptype::loyal,
    // );

    let new_user = Customer::new("Nouman".to_string()).build();
    let user_with_login = Customer::new("Joseph".to_string())
        .username("joe123".to_string())
        .build();

    let user_with_membership = Customer::new("Micheal".to_string())
        .username("micheal2000".to_string())
        .membership(Membershiptype::loyal)
        .build();
}
*/

// -------------------------------------------
//           	- Simplifying Structs
// -------------------------------------------
/*
// Starting code
struct A {
    f1: u32,
    f2: u32,
    f3: u32,
}
*/

struct A {
    f1: u32,
    f2: u32,
    f3: u32,
    // b: B,
    // c: C,
}

// -------------------------------------------
//           	- Simplifying structures
// -------------------------------------------

//The problem
// struct A {
//     f1: u32,
//     f2: u32,
//     f3: u32,
// }

// fn fn1(a: &mut A) -> &u32 {
//     &a.f2
// }
// fn fn2(a: &mut A) -> u32 {
//     a.f1 + a.f3
// }

// fn fn3(a: &mut A) {
//     let x = fn1(a);
//     let y = fn2(a);
//     println!("{}", x);
// }
// --------- Problem Ends ------
//---------- Solution --------
// struct A {
//     b: B,
//     c: C,
// }
// struct B {
//     f2: u32,
// }
// struct C {
//     f1: u32,
//     f3: u32,
// }

// fn fn1(b: &mut B) -> &u32 {
//     &b.f2
// }
// fn fn2(c: &mut C) -> u32 {
//     c.f1 + c.f3
// }

// fn fn3(a: &mut A) {
//     let x = fn1(&mut a.b);
//     let y = fn2(&mut a.c);
//     println!("{}", x);
// }

fn main() {}
