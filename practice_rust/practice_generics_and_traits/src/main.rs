/*
#[derive(Debug)]
struct Square {
    side: f32,
}

trait Shape {
    fn area(&self) -> f32;
}

impl Shape for Square {
    fn area(&self) -> f32 {
        self.side * self.side
    }
}

fn shape_properties(object: impl Shape + std::fmt::Debug) {
    println!("{:?}", object);
}

fn main() {

    let my_square = Square {
        side: 1.0
    };

    shape_properties(my_square);
}
*/

/*
#[derive(Debug)]
struct Square {
    side: f32,
}

trait Draw {
    fn draw_object(&self) {
        println!("default draw");
    }
}

trait Shape: Draw {
    fn area(&self) -> f32;
}

impl Draw for Square {
   99 fn draw_object(&self) {
        println!("draw for Square");
    }
}

impl Shape for Square {
    fn area(&self) -> f32 {
        self.side * self.side
    }
}

fn main() {
    let my_square = Square {
        side: 1.0
    };

    my_square.draw_object();
}
*/

/*
struct Square {
    side: f32,
}

struct Rectangle {
    length: f32,
    width: f32,
}

trait Shape {
    fn area(&self) -> f32;
    fn perimeter(&self) -> f32 {
        println!("Perimeter not implemented, returning dummy value");
        0.0
    }
}

impl Shape for Rectangle {
    fn area(&self) -> f32 {
        let area_of_rect = self.length * self.width;
        println!("Rectangle area: {}", area_of_rect);
        area_of_rect
    }
    fn perimeter(&self) -> f32 {
        let perimeter_of_rect = 2.0 * (self.length + self.width);
        println!("Rectangle Perimeter: {}", perimeter_of_rect);
        perimeter_of_rect
    }
}

impl Shape for Square {
    fn area(&self) -> f32 {
        let area_of_square = self.side * self.side;
        println!("Square area: {}", area_of_square);
        area_of_square
    }
}

// fn shape_properties_dynamic(object: Box<dyn Shape>) {
//     object.area();
//     object.perimeter();
// }

fn shape_properties_dynamic(object: &dyn Shape) {
    object.area();
    object.perimeter();
}

fn main() {
    let shape1 = Box::new(Rectangle {
        width: 5.0,
        length: 4.0,
    });

    let shape2 = Box::new(Square {
        side: 3.2,
    });
    // shape_properties_dynamic(shape1);
    // shape_properties_dynamic(shape2);
    
     shape_properties_dynamic(&shape1);
     shape_properties_dynamic(&shape2);
}
*/

// trait Properties: PartialEq + Default + Clone {}
// #[derive(Debug, PartialEq, Default, Clone)]
// struct Person {
//     name: String,
//     age: u8,
// }

// impl Properties for Person {}

// fn main() {
//     let person1 = Person {
//         name: String::from("Heungmin Son"),
//         age: 35,
//     };

//     let person2 = Person {
//         name: String::from("Jisong Park"),
//         age: 40,
//     };

//     println!("Person: {:?}", person1);
//     println!("Person: {:?}", person2);    
//     println!("person1 and person2 are equal: {}", person1 == person2);
// }

// #[derive(Debug, PartialEq, Default, Clone)]
// struct Person {
//     name: String,
//     age: u8,
// }


// fn main() {
//     let person1 = Person {
//         name: String::from("Heungmin Son"),
//         age: 35,
//     };

//     let person2 = Person {
//         name: String::from("Jisong Park"),
//         age: 40,
//     };

//     println!("Person: {:?}", person1);
//     println!("Person: {:?}", person2);    
//     println!("person1 and person2 are equal: {}", person1 == person2);
// }

// use std::fmt;

// struct Person {
//     name: String,
//     age: u8,
// }

// impl fmt::Debug for Person {
//     fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
//         f.debug_struct("Person")
//             .field("name", &self.name)
//             .field("age", &self.age)
//             .finish()
//     }
// }

// fn main() {
//     let person1 = Person {
//         name: String::from("Heungmin Son"),
//         age: 35,
//     };

//     let person2 = Person {
//         name: String::from("Jisong Park"),
//         age: 40,
//     };

//     println!("Person: {:?}", person1);
//     println!("Person: {:?}", person2);    
// }


// #[derive(Debug)]
// struct Person {
//     name: String,
//     age: u8,
// }

// // Default 트레이트를 직접 구현
// impl Default for Person {
//     fn default() -> Self {
//         Person {
//             name: String::from("Unknown"),
//             age: 0,
//         }
//     }
// }

// fn main() {
//     let default_person = Person::default();
//     println!("Default Person: {:?}", default_person);
// }


// `Send` 트레이트는 해당 타입이 다른 스레드로 안전하게 전송될 수 있음을 나타냅니다.
// 따라서 `Person` 구조체가 `Send`를 구현하면 이 구조체는 다른 스레드로 보낼 수 있습니다.
unsafe impl Send for Person {}

// `Sync` 트레이트는 해당 타입이 여러 스레드에서 안전하게 공유될 수 있음을 나타냅니다.
// 따라서 `Person` 구조체가 `Sync`를 구현하면 이 구조체는 여러 스레드에서 안전하게 공유될 수 있습니다.
unsafe impl Sync for Person {}

struct Person {
    name: String,
    age: u32,
}

fn main() {
    let person = Person {
        name: String::from("Alice"),
        age: 30,
    };

    // `Person` 구조체는 `Send`와 `Sync`를 구현하므로 다른 스레드로 전송하거나 여러 스레드에서 공유할 수 있습니다.
    //println!("Person: {:?}", person);
}




// -------------------------------------------
// 			Traits
// -------------------------------------------

/*
struct Square {
    side: f32,
}

struct Rectangle {
    length: f32,
    width: f32,
}

trait Shape {
    fn area(&self) -> f32;
}

impl Shape for Rectangle {
    fn area(&self) -> f32 {
        self.length * self.width
    }
}

impl Shape for Square {
    fn area(&self) -> f32 {
        self.side * self.side
    }
}

fn main() {
    let r1 = Rectangle {
        width: 5.0,
        length: 4.0,

    };

    let s1 = Square {
        side: 3.2,
    };

    r1.area();
    s1.area();
}
*/

#[derive(Debug)]
struct Person {
    name: String,
    age: u32,
}

trait Athlete { 
    type Sport;
    fn play(&self, sport: &Self::Sport);
}

impl Athlete for Person {
    type Sport = String;

    fn play(&self, sport: &Self::Sport) {
        println!("{} is playing {}!", self.name, sport);
    }
}

fn exercise<T: Athlete>(athlete: T, sport: T::Sport) {
    athlete.play(&sport);
}

fn main() {
    let person = Person {
        name: String::from("Heungmin Son"),
        age: 30,
    };

    let sport = String::from("Soccer");
    exercise(person, sport);
}




// -------------------------------------------
// 	Enums
// -------------------------------------------

/*
// Example 1: 

enum WeekDay {
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sundary,
}
fn main() {
    let mut day = "Saturday".to_string();

    let week_day = vec![
        "Monday".to_string(),
        "Tuesday".to_string(),
        "Wednesday".to_string(),
        "Thursday".to_string(),
        "Friday".to_string(),
        "Saturday".to_string(),
        "Sundary".to_string(),
    ];
    day = week_day[1].clone();

    let day = WeekDay::Saturday;
}*/

/*
// Example 2:
enum TravelType {
    Car(f32),
    Train(f32),
    Aeroplane(f32),
}

impl TravelType {
    fn travel_allowance(&self) -> f32 {
        let allowance = match self {
            TravelType::Car(miles) => miles * 2.0,
            TravelType::Train(miles) => miles * 3.0,
            TravelType::Aeroplane(miles) => miles * 5.0,
        };
        allowance
    }
}
fn main() {
    let participant = TravelType::Car(60.0);
    println!(
        "Allowance of participant is: {}",
        participant.travel_allowance()
    );
}
*/

/*
// Problem 1: Fix the code so that it compiles. 

trait Sound {
    fn animal_sound(&self) -> String { // Consider adding some code here 
        "no sound!".to_string()
    }
}
struct Dog;
struct Cat;     
struct Fish;

impl Sound for Dog {
    fn animal_sound(&self) -> String {
        "woof".to_string()
    }
}
impl Sound for Cat {
    fn animal_sound(&self) -> String {
        "meow".to_string()
    }
}
impl Sound for Fish {} // fish does not make any sound so we should not implement the 
                       //fn animal_sound(). This will make compiler unhappy 

fn main() {
    let dog = Dog;
    let cat = Cat;
    let fish = Fish;
    println!("Dog Sound: {}", dog.animal_sound());
    println!("Cat Sound: {}", cat.animal_sound());
    println!("Fish Sound: {}", fish.animal_sound());
}
*/

/*
// Problem 2: Fix the code by adding implementation for the Vehicle trait for the Bus and Bicycle types

trait Vehicle {
    fn speed(&self) -> f64 {
        0.0
    }
}

struct Car {
    model: String,
    speed: f64,
}

impl Vehicle for Car {
    fn speed(&self) -> f64 {
        self.speed
    }
}

// Do not change the struct definitions
struct Bicycle {
    brand: String,
}

struct Bus {
    model: String,
    speed: f64,
}

impl Vehicle for Bicycle { }

impl Vehicle for Bus {
    fn speed(&self) -> f64 {
        self.speed
    }
}

fn main() {
    let car = Car {
        model: "Camry".to_string(),
        speed: 120.0,
    };
    let bicycle = Bicycle {
        brand: "MountainBike".to_string(),
    };
    let bus = Bus {
        model: "BMC".to_string(),
        speed: 100.0,
    };

    car.speed();
    bicycle.speed();
    bus.speed();
}
*/

/*
// Problem: Make a few fixes to the code so that it compiles 

trait Greeting {
    fn greet(&self) -> String {
        "Hello from Rust!".to_string()
    }
}

fn print_greeting1<T: Greeting>(input: &T) {// Fix using trait bound
    println!("{}", input.greet());
}

fn print_greeting2(input: &impl Greeting) {// Fix using impl trait syntax 
    println!("{}", input.greet());
}

fn print_greeting3<T>(input: &T) 
// Fix by using the where clause
where 
    T: Greeting
{
    println!("{}", input.greet());
}

struct Greeter;
impl Greeting for Greeter {}

fn main() {
    let greeter_instance = Greeter;

    print_greeting1(&greeter_instance);
    print_greeting2(&greeter_instance);
    print_greeting3(&greeter_instance);
}
*/

/*
// Problem 2:  Fix the code by completing the function definition 

pub trait VehicleHorn {
    fn horn_sound(&self) -> String {
        "peep peep".to_string()
    }
}

struct Car {}

impl VehicleHorn for Car {
    fn horn_sound(&self) -> String {
        "peep".to_string()
    }
}

struct Truck {}

impl VehicleHorn for Truck {
    fn horn_sound(&self) -> String {
        "peeeeeeep".to_string()
    }
}

fn compare_horn_sound(vehicle_1: &impl VehicleHorn, vehicle_2: &impl VehicleHorn) -> bool { // complete the function definition 
    vehicle_1.horn_sound() == vehicle_2.horn_sound()
}

fn main() {
    let car = Car {};
    let truck = Truck {};
    assert_eq!(compare_horn_sound(&car, &truck), true);
}
*/

/*
// Problem 3: Complete the function signature of `get_square_root_str`.

trait SquareRoot {
    fn square_root(&self) -> Self;
}

trait Displayable {
    fn to_display_string(&self) -> String;
}

fn get_square_root_str(input: impl SquareRoot + Displayable) -> String { // make changes to this line only
    let squared_rooted = input.square_root();
    squared_rooted.to_display_string()
}

impl SquareRoot for f64 {
    fn square_root(&self) -> Self {
        self.sqrt()
    }
}

impl Displayable for f64 {
    fn to_display_string(&self) -> String {
        format!("{:.2}", self)
    }
}

fn main() {
    let num = 9.0;
    let mut msg = format!("{num} square rooted is ");
    msg.push_str(&get_square_root_str(num));
    println!("{msg}");
}
*/

/*
// Problem #1: Identify the error in the code and fix it

trait Drawable {
    fn draw(&self);
}

trait AnimatedDrawable: Drawable {
    fn animate(&self);
}

struct Circle;

impl Drawable for Circle {
    fn draw(&self) {
       println!("Drawing a circle"); 
    }
}

impl AnimatedDrawable for Circle {
    fn animate(&self) {
        println!("Animating a circle");
    }
}

fn main() {
    let circle = Circle;
    circle.draw();
    circle.animate();
}
*/

/*
// Problem 2: Try identifying the error in the code
// Hint: The error is related to the concept of supertrait

trait Size {
    fn compute_size(&self) -> u16;
}

trait Printable {
    fn size_to_str(&self) -> String;
}

trait Comparable : Size + Printable {
    fn print_greater(a: &Self, b: &Self) { 
    // Please note that Self on the line above means, that type which will be implementing the trait 

        let item1 = a.compute_size();
        let item2 = b.compute_size();
        if item1 > item2 {
            println!("{} is greater than {}", a.size_to_str(), b.size_to_str());
        } else if item2 > item1 {
            println!("{} is greater than {}", b.size_to_str(), a.size_to_str());
        } else {
            println!("Both sizes are {}", a.size_to_str());
        }
    }
}

struct Book {
    page: u16,
}

impl Size for Book {
    fn compute_size(&self) -> u16 {
        self.page
    }
}

impl Printable for Book {
    fn size_to_str(&self) -> String {
        format!("Book having {} pages", self.page)
    }
}

impl Comparable for Book {}

fn main() {
    let book_1 = Book { page: 50 };
    let book_2 = Book { page: 450 };
    Comparable::print_greater(&book_1, &book_2);
}
*/

/*
// Problem 1: fix the 'describe_animal' function signature and also fix the calls to it in main

trait Animal {
    fn name(&self) -> &str;
    fn make_sound(&self);
}

struct Lion {
    name: String,
}

impl Animal for Lion {
    fn name(&self) -> &str {
        &self.name
    }

    fn make_sound(&self) {
        println!("Roar!");
    }
}

struct Penguin {
    name: String,
}

impl Animal for Penguin {
    fn name(&self) -> &str {
        &self.name
    }

    fn make_sound(&self) {
        println!("Honk!");
    }
}

fn describe_animal(animal: &dyn Animal) { // This line needs a fix
    println!("The {} says:", animal.name());
    animal.make_sound();
}

fn main() {
    let lion = Lion {
        name: "Simba".to_string(),
    };
    let penguin = Penguin {
        name: "Happy Feet".to_string(),
    };

    // The calls to function needs fixes
    describe_animal(&lion);
    describe_animal(&penguin);
}
*/

/*
// Problem 2: Fix the code by making suitable changes to the signatures 
// of the functions 'get_vehicle' and 'operate_vehicle' 

trait Vehicle {
    fn start_engine(&self) -> String;
    fn drive(&self) -> String;
}

struct Car;

struct Bicycle;

impl Vehicle for Car {
    fn start_engine(&self) -> String {
        "🚗 Engine started".to_string()
    }

    fn drive(&self) -> String {
        "🚗 Driving the car".to_string()
    }
}

impl Vehicle for Bicycle {
    fn start_engine(&self) -> String {
        "🚲 No engine to start for a bicycle".to_string()
    }

    fn drive(&self) -> String {
        "🚲 Pedaling the bicycle".to_string()
    }
}

fn get_vehicle(vehicle_type: &str) -> Box<dyn Vehicle> { // This line needs a fix 
    match vehicle_type {
        "car" => Box::new(Car),
        "bicycle" => Box::new(Bicycle),
        _ => panic!("No vehicle of that type available"),
    }
}

fn operate_vehicle(driver: &dyn Vehicle) { // This line needs a fix 
    println!("{}", driver.start_engine());
    println!("{}", driver.drive());
}

fn main() {
    // Do not change code in main
    let my_vehicle = get_vehicle("car"); 
    operate_vehicle(my_vehicle.as_ref());

    let my_vehicle = get_vehicle("bicycle");
    operate_vehicle(my_vehicle.as_ref());
}
*/

/*
// Problem 3: Fix the code by adding a proper type annotation for the vector in main

trait Vehicle {
    fn start_engine(&self) -> String;
    fn drive(&self) -> String;
}

struct Car;

struct Bicycle;

impl Vehicle for Car {
    fn start_engine(&self) -> String {
        "🚗 Engine started".to_string()
    }

    fn drive(&self) -> String {
        "🚗 Driving the car".to_string()
    }
}

impl Vehicle for Bicycle {
    fn start_engine(&self) -> String {
        "🚲 No engine to start for a bicycle".to_string()
    }

    fn drive(&self) -> String {
        "🚲 Pedaling the bicycle".to_string()
    }
}

fn get_vehicle(vehicle_type: &str) -> Box<dyn Vehicle> {
    match vehicle_type {
        "car" => Box::new(Car),
        "bicycle" => Box::new(Bicycle),
        _ => panic!("No vehicle of that type available"),
    }
}

fn operate_vehicle(driver: &dyn Vehicle) {
    println!("{}", driver.start_engine());
    println!("{}", driver.drive());
}

fn main() {
    let vehicle_1 = Car;
    let vehicle_2 = Car;
    let vehicle_3 = Bicycle;

    let vehicles: Vec<&dyn Vehicle> = vec![&vehicle_1, &vehicle_2, &vehicle_3]; // Fix this line

    for v in vehicles {
        operate_vehicle(v);
    }
}
*/

/*
// Problem 1: Add the correct types for the associated type 'item' in the implemenation blocks
trait Container {
    type Item;

    fn add_item(&mut self, item: Self::Item);
    fn get_item(&self) -> Option<&Self::Item>;
}

struct VecContainer_i32 {
    items: Vec<i32>,
}

impl Container for VecContainer_i32 {
    type Item = i32; // This line needs a fix

    fn add_item(&mut self, item: Self::Item) {
        self.items.push(item);
    }

    fn get_item(&self) -> Option<&i32> {
        self.items.last()
    }
}

struct OptionContainer<T> {
    item: Option<T>,
}

impl<T> Container for OptionContainer<T> {
    type Item = T; // This line needs a fix

    fn add_item(&mut self, item: T) {
        self.item = Some(item);
    }

    fn get_item(&self) -> Option<&T> {
        self.item.as_ref()
    }
}

fn main() {
    let mut vec_container = VecContainer_i32 { items: Vec::new() };
    vec_container.add_item(42);
    vec_container.add_item(123);

    if let Some(last_item) = vec_container.get_item() {
        println!("Last item in VecContainer: {}", last_item);
    } else {
        println!("VecContainer is empty");
    }

    let mut option_container = OptionContainer { item: None };
    option_container.add_item("Hello, Rust!");

    if let Some(only_item) = option_container.get_item() {
        println!("Only item in OptionContainer: {}", only_item);
    } else {
        println!("OptionContainer is empty");
    }
}
*/

/*
// Problem 1: Add the associated types in the implementation blocks

#[derive(Debug)]
struct CarEngine {
    model: String,
    horsepower: u32,
}
#[derive(Debug)]
struct GasolineCar {}

#[derive(Debug)]
struct ElectricEngine {
    model: String,
    power: u32,
}

#[derive(Debug)]
struct ElectricCar {}

trait Vehicle {
    type EngineType;
    fn get_engine(&self) -> Self::EngineType;
}

impl Vehicle for GasolineCar {
    type EngineType = CarEngine; // Fix this line
    fn get_engine(&self) -> Self::EngineType {
        CarEngine {
            model: "V8".to_string(),
            horsepower: 400,
        }
    }
}

impl Vehicle for ElectricCar {
    type EngineType = ElectricEngine; // Fix this line
    fn get_engine(&self) -> Self::EngineType {
        ElectricEngine {
            model: "Electric Motor".to_string(),
            power: 300,
        }
    }
}

fn main() {
    let gasoline_car = GasolineCar {};
    let electric_car = ElectricCar {};

    println!("Gasoline Car engine: {:?}", gasoline_car.get_engine());
    println!("Electric Car engine: {:?}", electric_car.get_engine());
}
*/