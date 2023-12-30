use crate::garden::vegetables::Asparagus;
use crate::garden::vegetables::Onion;
use crate::garden::fruits::Fruits;

pub mod garden;

fn main() 
{
    let plant: Asparagus = Asparagus {};
    println!("I'm growing {:?}!", plant);

    let fruit = Fruits{};
    println!("I'm growing, too {:?}!", fruit);

    let onion = Onion{};
    println!("I'm growing, too {:?}!", onion);    
}