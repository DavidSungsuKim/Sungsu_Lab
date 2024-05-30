use occupation::Occupation; // submodule 이라도 module 사용선언이 필요함.

pub struct Person {
   pub id: u64,
   pub name: String,
   pub email: String,
   pub occupation: Occupation
}

pub mod occupation;   // person 의 submodule (src/person/occupation.rs)