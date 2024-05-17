// -------------------------------------------
// 	Structs and its Types
// -------------------------------------------

struct Car {
    owner: String,
    year: u32,
    fuel_level: f32,
    price: u32,
}

impl Car {
    fn monthly_insurance() -> u32 {
        123
    }

    fn selling_price(&self) -> u32 {
        self.price + Car::monthly_insurance()
    }

    fn new(name: String, year: u32) -> Self {
        Self {
            owner: name,
            year: year,
            fuel_level: 0.0,
            price: 0,
        }
    }

    fn display_car_info(&self) {
        println!(
            "Owner: {}, Year: {}, Price: {}",
            self.owner, self.year, self.price
        );
    }

    fn refuel(&mut self, gallons: f32) {
        self.fuel_level += gallons;
    }

    fn sell(self) -> Self {
        self
    }
}

fn main() {
    let mut my_car = Car {
        owner: String::from("ABC"),
        year: 2010,
        fuel_level: 0.0,
        price: 5_000,
    };

    let car_year = my_car.year;
    my_car.fuel_level = 30.0;
    let extracted_owner = my_car.owner.clone();
    println!("Owner is: {}", my_car.owner);

    let another_car = Car {
        owner: "new_name".to_string(),
        ..my_car
    };

    //println!("Owner is: {}", my_car.owner);

    // Tuple Structs
    let point_2D = (1, 3);
    let point_3D = (4, 10, 13);

    struct Point_2D(i32, i32);
    struct Point_3D(i32, i32, i32);

    let point1 = Point_2D(1, 3);
    let point2 = Point_3D(4, 10, 13);

    // Unit Struct
    struct ABC;

    let mut my_car = Car {
        owner: String::from("ABC"),
        year: 2010,
        fuel_level: 0.0,
        price: 5_000,
    };

    my_car.display_car_info();
    // display_car_info(&my_car);

    my_car.refuel(10.5);
    let new_owner = my_car.sell();
    // my_car.refuel(10.5);

    let new_car = Car::new("XYZ".to_string(), 2020);
}