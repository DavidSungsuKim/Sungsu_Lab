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
