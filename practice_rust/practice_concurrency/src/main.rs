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

/*
//Problem 2: Complete the code below

use std::thread;

fn main() {
    let handle_1 = thread::spawn(|| {
        let mut sum = 0;
        let range = 0..=1000;
        for num in range {
            sum += num;
        }
        sum
    }); // Note: The thread spawn returns a joinhandle type. If there is anything returned from
        // closure inside the thread, it will be inside the joinhandle type. In this case, it will be Joinhandle<i32>.
        // You can access the returned i32 value by calling .unwrap() on join.

    // Todo!: Insert a code for creating another thread which will compute the summation from 1001 - 2000
    let handle_2 = thread::spawn(|| {
        let mut sum = 0;
        let range = 1001..=2000;
        for num in range {
            sum += num;
        }
        sum
    });

    // Todo!: Insert a code for creating another thread which will compute the summation from 2001 - 3000
    let handle_3 = thread::spawn(|| {
        let mut sum = 0;
        let range = 2001..=3000;
        for num in range {
            sum += num;
        }
        sum
    });

    let mut sum = handle_1.join().unwrap() + handle_2.join().unwrap() + handle_3.join().unwrap();

    // Todo!: Insert code to make sure that the summation is computed correctly.
    // Summation will be computed correctly, if all the threads go to completion.

    println!("Final Summation Result {sum}");
}
*/

/*
// -------------------------------------------
// 			    - Multiple Threads
// 			    - Ownership and Threads
// -------------------------------------------

use std::thread::{self, spawn};
fn main() {
    // let mut thread_vec = vec![];
    // for i in 0..10 {
    //     thread_vec.push(thread::spawn(move || {
    //         println!("Thread number {}", i);
    //     }));
    // }

    // for i in thread_vec {
    //     i.join();
    // }

    let v = vec![1, 2, 3];
    let x = 5;
    let handle = thread::spawn(move || {
        println!("Here's a vector: {:?}", v);
        println!("Here's a variable : {:?}", x);
    });

    drop(x);
    println!("The variable x is still alive {}", x);
    //println!("The variable v is not alive {:?}", v);
    handle.join();
}
*/

/*
// Problem 1: Fix the code to make it compile.

use std::thread;

fn main() {
    let mut v = vec!["Nouman".to_string()];
    let handle = thread::spawn(move || {
        v.push("Azam".to_string());
    });
}
*/

/*
// Problem 1: Fix the code to make it compile.

use std::thread;

fn main() {
    let v = vec![1, 2, 3];
    let x = 5;

    let handle = thread::spawn(move || {
        println!("Here's a vector: {:?}", v);
        println!("Here's a variable : {:?}", x);
    });

    println!("The variable x is still alive {}", x);
    //println!("The variable v is not alive {}", v); // something wrong here <-- this can't be done because v is moved to the thread
    handle.join();
}
*/

/*
// -------------------------------------------
// 			Message Passing through Channels
// -------------------------------------------
use std::sync::mpsc;
use std::thread;
fn main() {
    let (tx, rx) = mpsc::channel();

    // let rx1 = rx;

    let t = thread::spawn(move || {
        let val = String::from("some data from sender");
        println!("Value sending from the thread");
        tx.send(val).unwrap();
        // println!("This may execute after the statement in the main");
        // println!("Val is {:?}", val);
    });

    // let recieved = rx.recv().unwrap();
    // println!("Recieved: {:?}", recieved);

    t.join();
    let mut recieved_status = false;
    while recieved_status != true {
        match rx.try_recv() {
            Ok(recieved_value) => {
                println!("Recieved value is {:?}", recieved_value);
                recieved_status = true;
            }
            Err(_) => println!("I am doing some other stuff"),
        }
    }
}
*/

// -------------------------------------------
// 			    - Sending Multiple Messages
// 			    - Multiple Producers
// 			    - Threads and Functions
// -------------------------------------------

/*
use std::sync::mpsc;
use std::thread;
fn main() {
    let (tx, rx) = mpsc::channel();

    let t = thread::spawn(move || {
        let my_vec = vec![1, 2, 3, 4, 5];
        for i in my_vec {
            tx.send(i).unwrap();
        }
    });

    for recieved_vals in rx {
        println!("I recieved the value of {}", recieved_vals);
    }

    /*
    let recived_vals_vec  = rx.iter().collect::<Vec<i32>>();
    println!("The recieved values are {:?}", recived_vals_vec);
     */
}
*/

/*
use std::sync::mpsc;
use std::thread;
use std::time::Duration;

fn main() {
    let (tx, rx) = mpsc::channel();
    let tx1 = tx.clone();

    thread::spawn(move || {
        let my_vec = vec![1, 2, 3, 4, 5];
        for i in my_vec {
            tx.send(i).unwrap();
            thread::sleep(Duration::from_secs(1));
        }
    });

    thread::spawn(move || {
        let my_vec = vec![6, 7, 8, 9, 10];
        for i in my_vec {
            tx1.send(i).unwrap();
            thread::sleep(Duration::from_secs(1));
        }
    });

    for recieved_vals in rx {
        println!("I recieved the value of {}", recieved_vals);
    }
}
*/

/*
use std::sync::mpsc;
use std::thread;
use std::time::Duration;

fn timer(d: i32, tx: mpsc::Sender<i32>) {
    thread::spawn(move || {
        //thread::sleep(Duration::from_millis(d as u64));
        println!("{} send!", d);
        tx.send(d).unwrap();
    });
}

fn main() {
    let (tx, rx) = mpsc::channel();
    for i in 0..5 {
        timer(i, tx.clone());
    }

    drop(tx); // this will be written at the end

    for recieving_val in rx {
        println!("{} recieved!", recieving_val);
    }
}
*/

// Problem 1: Fix the code below
/* original code
use std::sync::mpsc;
use std::thread;

fn main() {
    let (tx, rx) = mpsc::channel();

    thread::spawn(move || {
        let my_vec = vec![1, 2, 3, 4, 5];
        for i in my_vec {
            tx.send(i).unwrap();
        }
    });

    thread::spawn(move || {
        let my_vec = vec![6, 7, 8, 9, 10];
        for i in my_vec {
            tx.send(i).unwrap();   // fix this line
        }
    });

    for recieved_vals in rx {
        println!("I recieved the value of {}", recieved_vals);
    }
}
*/

/*
use std::sync::mpsc;
use std::thread;

fn main() {
    let (tx, rx) = mpsc::channel();

    let tx1 = tx.clone(); // Clone the sender for the second thread

    thread::spawn(move || {
        let my_vec = vec![1, 2, 3, 4, 5];
        for i in my_vec {
            tx.send(i).unwrap();
        }
    });

    thread::spawn(move || {
        let my_vec = vec![6, 7, 8, 9, 10];
        for i in my_vec {
            tx1.send(i).unwrap(); // Use the cloned sender for the second thread
        }
    });

    for recieved_vals in rx {
        println!("I recieved the value of {}", recieved_vals);
    }
}
*/

// Problem 2: Complete the code below
/* original code
use std::sync::mpsc;
use std::thread;

fn thread_fn(d: i32, tx: mpsc::Sender<i32>) {
    thread::spawn(move || {
        println!("{} send!", d);
        // Add code for sending d
    });
}

fn main() {
    let (tx, rx) = mpsc::channel();
    for i in 0..5 {
        // Add code for calling the function with value i and a clone of tx
    }

    drop(tx);

    for recieving_val in rx {
        println!("{} received!", recieving_val);
    }
}
*/

/*
use std::sync::mpsc;
use std::thread;

fn thread_fn(d: i32, tx: mpsc::Sender<i32>) {
    thread::spawn(move || {
        println!("{} send!", d);
        // Add code for sending d
        tx.send(d).unwrap();
    });
}

fn main() {
    let (tx, rx) = mpsc::channel();
    for i in 0..5 {
        // Add code for calling the function with value i and a clone of tx
        let tx2 = tx.clone();
        thread_fn(i, tx2);
    }

    drop(tx);

    for recieving_val in rx {
        println!("{} received!", recieving_val);
    }
}
*/

// Problem 1: The code is unable to do some other stuff.
// Seems like a message is always available, when you make a call to try_recv().
// Fix the code so that it is able to do other work.

/* original code
use std::sync::mpsc;
use std::thread;
use std::time::Duration;
fn main() {
    let (tx, rx) = mpsc::channel();

    let t = thread::spawn(move || {
        let x = "some_value".to_string();
        println!("Sending value {x}");
        tx.send(x).unwrap();
    });

    t.join(); // Something wrong here
    let mut received_status = false;
    while received_status != true {
        match rx.try_recv() {
            Ok(received_value) => {
                println!("Received value is: {received_value}");
                received_status = true;
            }
            Err(_) => println!("I am doing some other stuff"), // This line never executes.
                                                               // Make approperiate changes in the code, so that this line executes.
        }
    }
}
*/

/*
use std::sync::mpsc;
use std::thread;
use std::time::Duration;
fn main() {
    let (tx, rx) = mpsc::channel();

    let t = thread::spawn(move || {
        let x = "some_value".to_string();
        println!("Sending value {x}");
        tx.send(x).unwrap();
    });

    let mut received_status = false;
    while received_status != true {
        match rx.try_recv() {
            Ok(received_value) => {
                println!("Received value is: {received_value}");
                received_status = true;
            }
            Err(_) => println!("I am doing some other stuff"), // This line never executes.
                                                               // Make approperiate changes in the code, so that this line executes.
        }
    }

    t.join(); // Something wrong here
}
*/

/*
// -------------------------------------------
// 			Sharing States
// -------------------------------------------
use std::sync::Mutex;
fn main() {
    let m = Mutex::new(5);

    /*
    {
        let mut num = m.lock().unwrap();
        *num = 10;
    }
    */

    println!("m = {:?}", m);

    let mut num = m.lock().unwrap();
    *num = 10;
    drop(num);

    let mut num1 = m.lock().unwrap();
    *num1 = 15;
    drop(num1);
}
*/

// -------------------------------------------
// 			Passes Mutexes betwen Threads
// -------------------------------------------

/*
use std::sync::Mutex;
use std::thread;
//use std::rc::Rc;
//use std::sync::Arc;

fn main() {
    let counter = Mutex::new(0);
    //let counter = Rc::new(Mutex::new(0));
    //let counter = Arc::new(Mutex::new(0));

    let mut handles = vec![];

    for _ in 0..10 {
        //let counter = Rc::clone(&counter);
        //let counter = Arc::clone(&counter);
        let handle = thread::spawn(move || {
            let mut num = counter.lock().unwrap();

            *num += 1;
        });
        handles.push(handle);
    }

    for handle in handles {
        handle.join().unwrap();
    }

    println!("Result: {}", *counter.lock().unwrap());
}
*/

/*
use std::sync::Arc;
use std::thread;

struct MyString(String);

impl MyString {
    fn new(s: &str) -> MyString {
        MyString(s.to_string())
    }
}

fn main() {
    let mut threads = Vec::new();
    let name = Arc::new(MyString::new("Rust"));

    for i in 0..5 {
        let some_str = name.clone();
        let t = thread::spawn(move || {
            println!("hello {} count {}", some_str.0, i);
        });
        threads.push(t);
    }

    for t in threads {
        t.join();
    }
}
*/

// -------------------------------------------
// 			    - Barriers
// -------------------------------------------

/*
use std::sync::Arc;
use std::sync::Barrier;
use std::thread;

fn main() {
    let mut threads = Vec::new();
    let barrier = Arc::new(Barrier::new(5));

    for i in 0..10 {
        let barrier = barrier.clone();
        let t = thread::spawn(move || {
            println!("before wait {}", i);
            barrier.wait();
            println!("after wait {}", i);
        });
        threads.push(t);
    }

    for t in threads {
        t.join().unwrap();
    }
}
*/

use std::sync::Arc;
use std::sync::Barrier;
use std::sync::Mutex;
use std::thread;

fn main() {
    let mut threads = Vec::new();
    let barrier = Arc::new(Barrier::new(3));
    let data = Arc::new(vec![
        vec![1, 2, 3, 4, 5, 6],
        vec![1, 2, 3, 4, 5, 6],
        vec![1, 2, 3, 4, 5, 6],
    ]);

    let result = Arc::new(Mutex::new(0));

    for i in 0..3 {
        let barrier = barrier.clone();
        let data = data.clone();
        let result = result.clone();
        let t = thread::spawn(move || {
            let x: i32 = data[i][0..3].iter().sum();
            *result.lock().unwrap() += x;

            //let mut x = result.lock().unwrap();
            //*x = data[i][0..3].iter().sum();

            println!("Thread {} Part 1 is done", i);
            barrier.wait();

            let x: i32 = data[i][3..6].iter().sum();
            *result.lock().unwrap() += x;
            //*x = data[i][3..6].iter().sum();

            println!("Thread {} is complete ", i);
        });
        threads.push(t);
    }

    for t in threads {
        t.join().unwrap();
    }

    println!(
        "The final value of hte result is {}",
        *result.lock().unwrap()
    );
}
