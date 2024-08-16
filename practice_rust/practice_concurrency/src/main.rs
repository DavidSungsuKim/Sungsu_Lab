/*
// -------------------------------------------
// 			    - Threads Basics
// -------------------------------------------
use std::thread;
use std::time::Duration;

fn main() {
    println!("This will be printed");
    println!("This will also be printed");
    println!("The concurrency will start after this line");

    let t = thread::spawn(|| {
        println!("Hello 1 from the thread");
        println!("Hello 2 from the thread");
        println!("Hello 3 from the thread");
        println!("Hello 4 from the thread");
        println!("Hello 5 from the thread");
        println!("Hello 6 from the thread");
        println!("Hello 7 from the thread");
    });

    thread::sleep(Duration::from_millis(1));
    println!("Hello 1 from the main");
    println!("Hello 2 from the main");
    t.join();
}
*/

// Problem 1: Complete the code below.

/*
// original code given
use std::thread;

fn main() {

    let mut thread_vec = vec![];
    for i in 0..10 {
        thread_vec.push(
        // insert code here
        // Spawn a thread
        // include a simple print statement with a msg of "Hi from Thread"
        );
    }

    // The code below will make sure that all the threads go to completion
    for i in thread_vec {
        i.join();
    }
}
*/

/*
use std::thread;

fn main() {
    let mut thread_vec = vec![];
    for i in 0..10 {
        thread_vec.push(thread::spawn(|| {
            println!("Hi from Thread");
        }));
    }

    for i in thread_vec {
        i.join();
    }
}
*/

//Problem 2: Complete the code below

use std::thread;

fn main() {
    let handle_1 = thread::spawn(|| {
        let mut sum = 0;
        let range = 0..=1_000;
        for num in range {
            sum += num;
        }
        sum
    }); // Note: The thread spawn returns a joinhandle type. If there is anything returned from
        // closure inside the thread, it will be inside the joinhandle type. In this case, it will be Joinhandle<i32>.
        // You can access the returned i32 value by calling .unwrap() on join.

    // Todo!: Insert a code for creating another thread which will compute the summation from 1001 - 2000

    // Todo!: Insert a code for creating another thread which will compute the summation from 2001 - 3000

    let mut sum = 0;

    // Todo!: Insert code to make sure that the summation is computed correctly.
    // Summation will be computed correctly, if all the threads go to completion.

    println!("Final Summation Result {sum}");
}
