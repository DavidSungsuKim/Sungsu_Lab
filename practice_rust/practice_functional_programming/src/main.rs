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

