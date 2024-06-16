/*
// -------------------------------------------
// 			Closures
// -------------------------------------------

struct User {
    name: String,
    age: u8,
    salary: u32,
}

// fn validate_user(name: &str) -> bool {
//     name.len() != 0
// }

fn is_valid_user<V1, V2>(name: &str, age: u8, simple_validator: V1, advance_validator: V2) -> bool
where
    V1: FnOnce(&str) -> bool,
    V2: Fn(u8) -> bool,
{
    simple_validator(name) && advance_validator(age)
}
fn main() {
    let person_1 = User {
        name: String::from("someone"),
        age: 35,
        salary: 40_000,
    };

    let mut banned_user = String::from("banned user");
    let validate_user_simple = move |name: &str| {
        let banned_user_name = &banned_user;
        name.len() != 0 && name != banned_user_name
    };
    //println!("{banned_user}");

    let validate_user_advance = |age: u8| age >= 30;
    println!(
        "User validity {}",
        is_valid_user(
            &person_1.name,
            person_1.age,
            validate_user_simple,
            validate_user_advance
        )
    );
}
*/

/*
// Simple example
fn is_valid_user<T>(name: &str, validator: T) -> bool
where
    T: FnOnce(&str) -> bool,
{
    validator(name)
}

fn main() {
    let user = String::from("someone");

    let validate_user = move |name: &str| {
        let user_filtered = String::from("invalid user");
        name.len() != 0 && name != user_filtered
    };

    println!("{}", is_valid_user( &user.to_string(), validate_user ));
}
*/

/*
// Simple example
fn is_valid_user(name: &str, validator: fn(&str) -> bool) -> bool
{
    validator(name)
}

fn main() {
    let user = String::from("someone");

    let validate_user = move |name: &str| {
        let user_filtered = String::from("invalid user");
        name.len() != 0 && name != user_filtered
    };

    println!("{}", is_valid_user( &user.to_string(), validate_user ));
}
*/

/*
fn validate_user (name: &str) -> bool {
    let user_filtered = String::from("invalid user");
    name.len() != 0 && name != user_filtered
}

fn is_valid_user(name: &str, validator: fn(&str) -> bool) -> bool
{
    validator(name)
}

fn main() {
    let user = String::from("someone");
    println!("{}", is_valid_user( &user.to_string(), validate_user ));
}
*/

/*
fn validate_user (name: &str) -> bool {
    let user_filtered = String::from("invalid user");
    name.len() != 0 && name != user_filtered
}

fn is_valid_user<T>(name: &str, validator: T) -> bool
where
    T: FnOnce(&str) -> bool,
{
    validator(name)
}

fn main() {
    let user = String::from("someone");

    // let validate_user = move |name: &str| {
    //     let user_filtered = String::from("invalid user");
    //     name.len() != 0 && name != user_filtered
    // };

    println!("{}", is_valid_user( &user.to_string(), validate_user ));
}
*/

/*
fn main() {
    let is_plus_integer = |value: u8| value > 0;
    println!("result={}", is_plus_integer(1));
}
*/

/*
fn main() {
    let mut sum = 0;
    let mut addup = |value_to_add : u32| sum += value_to_add;
    addup(1);
    addup(1);
    addup(1);
    println!("result={:?}", sum);
}
*/

/*
// Problem 1: Complete the code by adding the closure definition

fn main() {
    let x = 10;
    let add_to_x = |y| { x + y }; /* Add closure definition here */

    let result = add_to_x(5);
    println!("Result: {}", result);
}
*/

/*
// Problem 2: Complete the 'process_employee' function signature by adding the suitable trait bounds

struct Employee {
    name: String,
    salary: u32,
    department: String,
}

fn process_employees<V1, V2>(
    employees: Vec<Employee>,
    name_transformer: V1,
    salary_filter: V2,
) -> Vec<String>
where
    V1: Fn(&str) -> String,
    V2: Fn(u32) -> bool
{
    let mut processed_names = Vec::new();

    for employee in employees {
        if salary_filter(employee.salary) {
            processed_names.push(name_transformer(&employee.name));
        }
    }

    processed_names
}

fn main() {
    let employees = vec![
        Employee {
            name: String::from("Alice"),
            salary: 60000,
            department: String::from("Engineering"),
        },
        Employee {
            name: String::from("Bob"),
            salary: 75000,
            department: String::from("Sales"),
        },
        Employee {
            name: String::from("Charlie"),
            salary: 50000,
            department: String::from("Marketing"),
        },
    ];

    let transform_name_to_uppercase = |name: &str| name.to_uppercase();

    let filter_salary_above_threshold = |salary: u32| salary > 60000;

    let processed_names = process_employees(
        employees,
        transform_name_to_uppercase,
        filter_salary_above_threshold,
    );

    println!("Processed names: {:?}", processed_names);
}
*/

/*
// Problem 3: Complete the code by adding the closure definition

fn main() {
    let mut counter = 0;

    let mut increment_counter = || { counter += 1 }; /* Complete the Closure definition */
    increment_counter();
    increment_counter();

    println!("Final Counter: {}", counter);
}
*/

/*
// Problem 4: Fix the struct definition to allow closures with event handling logic.

struct EventHandler<T>
where
    T: FnMut(), // Something wrong here.
             //Hint: Check the code in main and see how the closure is using
             // the values from its enviroment
{
    on_event: T,
}

impl<T> EventHandler<T>
where
    T: FnMut(), // Something wrong here
{
    fn handle_event(&mut self) {
        (self.on_event)()
    }
}

fn main() {
    let mut lights_on = false;
    let mut temperature = 25;

    let mut lights_handler = EventHandler {
        on_event: || {
            lights_on = !lights_on;
            println!("Lights are now {}", if lights_on { "on" } else { "off" });
        },
    };

    let mut temperature_handler = EventHandler {
        on_event: || {
            temperature += 5;
            println!("Temperature increased to {}°C", temperature);
        },
    };

    lights_handler.handle_event();
    temperature_handler.handle_event();
    temperature_handler.handle_event();
    lights_handler.handle_event();

    assert_eq!(temperature, 35);
    assert_eq!(lights_on, true);
}
*/

/*
// Problem 1: Complete the function signature for `sum_of_squares`.
//It must not contain any generics.

fn add(x: u32, y: u32) -> u32 {
    x + y
}

fn square(x: u32) -> u32 {
    x * x
}

fn sum_of_squares(num: u32, sq: fn(u32) -> u32, add: fn(u32, u32) -> u32) -> u32 {
    let mut result = 0;
    for i in 1..=num {
        result = add(result, sq(i));
    }
    result
}

fn main() {
    let num = 4;
    let sum = sum_of_squares(num, square, add);
    println!("Sum of squares from 1 to {} = {}", num, sum);
}
*/

/*
// Problem 2: Update the function signature so that it uses function pointers instead of closures.
// You will also add a square function.

fn invoker(operation: fn(i32) -> i32, operand: i32) -> i32 // This needs to be updated
{
    operation(operand)
}

fn operation(op: i32) -> i32 {
    op * op
}

/* A square function needs to be added here */

fn main() {
    let square = |x: i32| x * x;

    let result = invoker(square, 4);
    println!("Result is: {}", result);
}
*/

/*
// -------------------------------------------
// 			Iterator
// -------------------------------------------

// trait Iterator {
//     type Item;
//     fn next(&mut self) -> Option<Self::Item>;
// }

struct Employee {
    name: String,
    salary: u16,
}

struct Employee_Records {
    employee_db: Vec<Employee>,
}

impl Iterator for Employee_Records {
    type Item = String;
    fn next(&mut self) -> Option<Self::Item> {
        if self.employee_db.len() != 0 {
            let result = self.employee_db[0].name.clone();
            self.employee_db.remove(0);
            Some(result)
        } else {
            None
        }
    }
}
fn main() {
    let mut emp_1 = Employee {
        name: String::from("John"),
        salary: 40_000,
    };

    let mut emp_2 = Employee {
        name: String::from("Joseph"),
        salary: 30_000,
    };

    let mut emp_db = Employee_Records {
        employee_db: vec![emp_1, emp_2],
    };

    // println!("{:?}", emp_db.next());
    // println!("{:?}", emp_db.next());
    // println!("{:?}", emp_db.next());

    for employee in emp_db {
        println!("{employee}");
    }
}
*/

// -------------------------------------------
// 			Iterator
// -------------------------------------------

/*
// trait Iterator {
//     type Item;
//     fn next(&mut self) -> Option<Self::Item>;
// }

#[derive(Debug)]
struct Employee {
    name: String,
    salary: u16,
}

struct Employee_Records {
    employee_db: Vec<Employee>,
}

// impl Iterator for Employee_Records {
//     type Item = String;
//     fn next(&mut self) -> Option<Self::Item> {
//         if self.employee_db.len() != 0 {
//             let result = self.employee_db[0].name.clone();
//             self.employee_db.remove(0);
//             Some(result)
//         } else {
//             None
//         }
//     }
// }

fn main() {
    let mut emp_1 = Employee {
        name: String::from("John"),
        salary: 40_000,
    };

    let mut emp_2 = Employee {
        name: String::from("Joseph"),
        salary: 30_000,
    };

    let mut emp_db = Employee_Records {
        employee_db: vec![emp_1, emp_2],
    };

    for employee in emp_db.employee_db.iter() {
        println!("{:?}", employee);
    }
}

*/

/*
fn main() {
    let vec = vec![1, 2, 3, 4, 5];
    let mut iter = vec.iter();
    for val in iter {
        println!("{}", val);
    }
}
*/

fn main() {
    let vec = vec![1, 2, 3, 4, 5];
    let mut iter = vec.iter();

    println!("{}", iter.next().unwrap());
    println!("{}", iter.next().unwrap());
    println!("{}", iter.next().unwrap());
    println!("{}", iter.next().unwrap());
    println!("{}", iter.next().unwrap());

    // for val in iter {
    //     println!("{}", val);
    // }
}

/*
// Problem 1: Compile the code by adding the definition for the next method

struct Counter {
    current: u32,
    max: u32,
}

impl Counter {
    fn new(max: u32) -> Counter {
        Counter { current: 0, max }
    }
}

impl Iterator for Counter {
    type Item = u32;

    fn next(&mut self) -> Option<Self::Item> {
        if self.current < self.max {
            let current = Some(self.current);
            self.current += 1;
            current
        }
        else {
            None
        }
    }
}

fn main() {
    let mut counter = Counter::new(3);
    assert!(matches!(counter.next(), Some(0)));
    assert!(matches!(counter.next(), Some(1)));
    assert!(matches!(counter.next(), Some(2)));
    assert!(matches!(counter.next(), None));
}

*/

/*
// Problem 2: Complete the into_iter function definition
// Solution:

struct Person {
    name: String,
    age: u32,
    occupation: String,
}

impl IntoIterator for Person {
    type Item = String;
    type IntoIter = std::vec::IntoIter<Self::Item>;

    fn into_iter(self) -> Self::IntoIter {
        vec![self.name, self.age.to_string(), self.occupation].into_iter()
    }
}

fn main() {
    let person = Person {
        name: "Alice".to_string(),
        age: 30,
        occupation: "Software Engineer".to_string(),
    };

    let mut person_iterator = person.into_iter();

    while let Some(property) = person_iterator.next() {
        println!("{}", property);
    }
}
*/

/*
// Problem 3: Complete the into_iter function definition.
// Also add the type for the associated type item
// Solution:

struct Pixel {
    r: i8,
    g: i8,
    b: i8,
}

impl IntoIterator for Pixel {
    type Item = i8;
    type IntoIter = std::vec::IntoIter<Self::Item>;

    fn into_iter(self) -> Self::IntoIter {
        vec![self.r, self.g, self.b].into_iter()
    }
}

fn main() {
    let p = Pixel {
        r: 54,
        g: 23,
        b: 74,
    };
    let p = p.into_iter();

    for component in p {
        println!("{}", component);
    }
}
*/

/*
// Problem 1: Fix the code so that it compiles
// Solution:

fn main() {
    let mut vec_1 = vec![4, 5, 6, 9, 8];
    for i in vec_1.iter_mut()  {
        *i = *i * 2;
    }
    println!("{:?}", vec_1);
}
*/

/*
// Problem 1: Convert the code based on the combinators

fn main() {
    let numbers = vec![1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    let mut result = 0;

    /* The code in the for loop needs to be replaced */
    for &num in &numbers {
        if num % 2 != 0 {
            let squared_num = num * num;
            result += squared_num;
        }
    }

    println!("Result without combinators: {}", result);

    let result = numbers
        .into_iter()
        .filter(|&num| num % 2 != 0 )
        .map(|num| num * num)
        .collect::<Vec<i32>>()
        .into_iter().sum::<i32>();

    println!("Result with combinators: {}", result);
}
*/

/*
// -------------------------------------------
//          - Iterating Through Options
// -------------------------------------------

fn main() {
    // ------ Use case 1 -----

    let some_product = Some("laptop");
    let mut products = vec!["cellphone", "battery", "charger"];

    // Solution 1:
    // match some_product {
    //     Some(product) => products.push(product),
    //     _ => {}
    // };

    // Solution 2:
    // if let Some(product) = some_product {
    //     products.push(product);
    // }

    // Solution 3:
    products.extend(some_product);
    println!("{:?}", products);

    // ------- Use case 2 -----
    let mut products = vec!["cellphone", "battery", "charger"];
    let products_iter = products.iter().chain(some_product.iter());

    for prod in products_iter {
        println!("{:?} ", prod);
    }

    // ------ Use Case 3 -----
    let products = vec![Some("charger"), Some("battery"), None, Some("cellphone")];

    // Solution 1;
    // let mut prod_without_none = Vec::new();
    // for p in products {
    //     if p.is_some() {
    //         prod_without_none.push(p.unwrap());
    //     }
    // }

    // Solution 2:
    // let prod_without_none = products
    //     .into_iter()
    //     .filter(|x| x.is_some())
    //     .map(|x| x.unwrap())
    //     .collect::<Vec<&str>>();

    // Solution 3:
    let prod_wihtout_none: Vec<&str> = products.into_iter().flatten().collect();
    println!("{:?}", prod_wihtout_none);
}
*/
