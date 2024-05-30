use learning_rust::person::Person;
use learning_rust::person::occupation::Occupation;

fn main() {
    println!("Hello world!");

    let mut myself = Person {
        id: 12345678,
        name: String::from("Embedded"),
        email: String::from("...@naver.com"),
        occupation: Occupation::EmbeddedDeveloper
    };

    println!("{},{:?},{:?}", 
    myself.id, myself.name, myself.email);

    match myself.occupation {
        Occupation::EmbeddedDeveloper => println!("Embedded guy"),
        _ => {}
    }
}