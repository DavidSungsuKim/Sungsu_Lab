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
        //let y = v;
        println!("Here's a vector: {:?}", y);
        //println!("Here's a variable : {:?}", x);
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

    // t.join();
    // let mut recieved_status = false;
    // while recieved_status != true {
    //     match rx.try_recv() {
    //         Ok(recieved_value) => {
    //             println!("Recieved value is {:?}", recieved_value);
    //             recieved_status = true;
    //         }
    //         Err(_) => println!("I am doing some other stuff"),
    //     }
    // }

    t.join();
    let mut received_value = rx.recv().unwrap();
    println!("Recieved value is {:?}", received_value);
}
*/

/*
// multiple senders
use std::sync::mpsc;
use std::thread;
fn main() {
    let (tx, rx) = mpsc::channel();
    let mut handles = vec![];

    for i in 0..10 {
        let tx_clone = tx.clone();
        let t = thread::spawn(move || {
            let val = String::from("a tx msg");
            tx_clone.send(val).unwrap();
        });
        handles.push(t);
    }

    for handle in handles {
        handle.join();
    }

    let mut num_of_messages = 0;
    while num_of_messages < 10 {
        match rx.try_recv() {
            Ok(recieved_value) => {
                println!("Recieved value is {:?}", recieved_value);
                num_of_messages += 1;
            }
            Err(_) => {}
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

/* This doesn't work; the author deliberately made it not work
// -------------------------------------------
// 			Passes Mutexes betwen Threads
// -------------------------------------------

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

    //println!("Result: {}", *counter.lock().unwrap());
}
*/

/*
use std::sync::{Arc, Mutex};
use std::thread;

struct MyString(String);

impl MyString {
    fn new(s: &str) -> MyString {
        MyString(s.to_string())
    }
}

fn main() {
    let name = Arc::new(Mutex::new(MyString::new("Rust")));
    let mut thread_handles = Vec::new();

    for i in 0..5 {
        let some_str = Arc::clone(&name);
        let t = thread::spawn(move || {
            let some_str = Arc::clone(&name).lock().unwrap();
            println!("str: {} count: {}", some_str.0, i);
        });
        thread_handles.push(t);
    }

    for t in thread_handles {
        t.join();
    }
}*/

/*
// -------------------------------------------
// 			    - Barriers
// -------------------------------------------

use std::sync::Arc;
use std::sync::Barrier;
use std::thread;

fn main() {
    let mut thread_handles = Vec::new();
    let barrier = Arc::new(Barrier::new(5));

    for i in 0..5 {
        let barrier = barrier.clone();
        let t = thread::spawn(move || {
            println!("before wait {}", i);
            barrier.wait();
            println!("after wait {}", i);
        });
        thread_handles.push(t);
    }

    for t in thread_handles {
        t.join().unwrap();
    }
}
*/

/*
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
            // *x = data[i][0..3].iter().sum();

            println!("Thread {} Part 1 is done", i);
            barrier.wait();

            let x: i32 = data[i][3..6].iter().sum();
            *result.lock().unwrap() += x;
            // *x = data[i][3..6].iter().sum();

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
*/

// -------------------------------------------
// 			        - Scope Threads
// -------------------------------------------
/* An example using thread::scope
use std::thread;

fn main() {
    let mut vec = vec![1, 2, 3];
    let mut x = 0;

    thread::scope(|some_scope| {
        some_scope.spawn(|| {
            println!("1st thread in the scope");
            println!("{:?}", vec);
        });

        some_scope.spawn(|| {
            println!("2nd thread in the scope");
            x += 1;
            //vec.push(4);
            println!("{:?}", vec);
        });
    });

    println!("All threads have been joined");
    x += 1;
    vec.push(5);
    println!("x: {:?} and vec: {:?}", x, vec);
}
*/

/* An example not using thread::scope
use std::thread;

fn main() {
    let mut vec = vec![1, 2, 3];
    let mut x = 0;

    let handle1 = thread::spawn({
        // 1st thread 생성
        let vec = vec.clone(); // vec 를 clone 하고 move 를 통해 thread 로 넘김
        move || {
            println!("1st thread");
            println!("{:?}", vec);
        }
    });

    let handle2 = thread::spawn({
        // 2nd thread 생성
        let vec = vec.clone(); // vec 를 clone 하고 move 를 통해 thread 로 넘김
        move || {
            println!("2nd thread");
            x += 1;
            // vec.push(4);
            println!("{:?}", vec);
        }
    });

    handle1.join().unwrap(); // join 을 호출하여 1st thread 가 끝나기를 명시적으로 대기
    handle2.join().unwrap(); // join 을 호출하여 1st thread 가 끝나기를 명시적으로 대기

    println!("All threads have been joined");
    x += 1;
    vec.push(5);
    println!("x: {:?} and vec: {:?}", x, vec);
}
*/

/*
// -------------------------------------------
// 			        - Thread Park
// -------------------------------------------

use std::thread;
use std::time::Duration;
fn main() {
    let job_1 = thread::spawn(|| {
        println!("-- Job 1 has started -- ");
        println!("Waiting for job 2 to complete");
        //thread::park_timeout(Duration::from_secs(2));
        //thread::sleep(Duration::from_secs(2));
        thread::yield_now();

        println!("-- Job 1 resumed --");
        println!("-- Job 1 finished");
    });

    let job_2 = thread::spawn(|| {
        println!("-- Job 2 started --");
        println!(" -- Job 2 finished --");
    });
    job_2.join().unwrap();
    println!("Job 2 is now completed");
    println!("Job 1 will now resume");
    job_1.thread().unpark();
    job_1.join().unwrap();
}
*/

/*
// -------------------------------------------
// 			    - Async Await basics
//                   - tokio = {version = "1.17", features = ["full"]}
// -------------------------------------------

async fn printing() {
    println!("I am async function");
}

#[tokio::main]
async fn main() {
    // let x = printing();
    // println!("The has not being polled yet");
    // drop(x);

    let x = printing();
    println!("The has not being polled yet");
    x.await;
}
*/

// -------------------------------------------
// 			    - Async Await (Tasks, Select)
//                  - tokio = {version = "1.17", features = ["full"]}
// -------------------------------------------

/*
#[tokio::main]
async fn main() {
    let mut handles = vec![];
    println!("This code is not part of the async block");
    let s1 = String::from("Huge Computation function");
    let s2 = String::from("Simpler Computation function");
    let aw1 = tokio::spawn(async move {
        huge_computation(s1).await;
    });
    handles.push(aw1);

    let aw2 = tokio::spawn(async move {
        simpler_computation(s2).await;
    });
    handles.push(aw2);

    for handle in handles {
        handle.await.unwrap();
    }
    println!("All tasks are now completed");
}

async fn huge_computation(s: String) {
    println!("{:?} has started", s);
    for i in 0..100_000_000 {}
    println!("{:?} is now completed", s);
}
async fn simpler_computation(s: String) {
    println!("{:?} has started", s);
    println!("{:?} is now completed", s);
}
*/

/*
use tokio::select;
#[tokio::main]
async fn main() {
    // tokio::select! {
    //     _ = function_1() => println!("Function 1 is completed first"),
    //     _ = function_2() => println!("Function 2 is completed first"),
    // };

    // let aw1 = tokio::spawn(async move {
    //     function_1().await;
    // });

    // let aw2 = tokio::spawn(async move {
    //     function_2().await;
    // });

    // tokio::select! {
    //     _ = aw1 => println!("Function 1 is selected"),
    //     _ = aw2 => println!("Function 2 is selected"),
    // };

    tokio::join! {
        function_1(),
        function_2(),
    };
}

async fn function_1() {
    println!("Function 1 has started");
    for i in 0..100_000_000 {}
    println!("Function 1 has ended");
}

async fn function_2() {
    println!("Function 2 has started");
    println!("Function 2 has ended");
}
*/

/*
// Problem 1: Fix the code to make it compile. You may only add code, not remove it.

use std::time::Duration;
use tokio::time::sleep;

struct Project {
    id: u32,
    name: String,
    duration_days: u32,
}

impl Project {
    fn new(id: u32, name: &str, duration_days: u32) -> Self {
        Self {
            id,
            name: name.to_string(),
            duration_days,
        }
    }
}

#[tokio::main]
async fn main() {
    let (id1, id2) = (1, 2);
    let project1 = read_details_from_db(id1).await.unwrap();
    let project2 = read_details_from_db(id2).await.unwrap();
    if project1.duration_days > project2.duration_days {
        println!(
            "{} takes {} days more than {}",
            project1.name,
            project1.duration_days - project2.duration_days,
            project2.name
        );
    } else if project2.duration_days > project1.duration_days {
        println!(
            "{} takes {} days more than {}",
            project2.name,
            project2.duration_days - project1.duration_days,
            project1.name
        );
    } else {
        println!(
            "Both {} and {} take the same number of days",
            project1.name, project2.name
        );
    }
}

async fn read_details_from_db(id: u32) -> Result<Project, String> {
    // dummy read from database
    sleep(Duration::from_millis(1000)).await;
    let database = [
        Project::new(1, "Project Alpha", 30),
        Project::new(2, "Project Beta", 45),
        Project::new(3, "Project Gamma", 30),
    ];
    for project in database {
        if id == project.id {
            return Ok(project);
        }
    }
    Err("Project record not present".into())
}
*/

// Problem 2: Fix the code so that the two functions execute concurrently.
// Consider calling the functions inside spawned tasks.

/* original code
use tokio::time::{sleep, Duration};

async fn fn1() {
    println!("Task 1 started!");
    sleep(Duration::from_secs(3)).await;
    println!("Task 1 completed!");
}

async fn fn2() {
    println!("Task 2 started!");
    sleep(Duration::from_secs(2)).await;
    println!("Task 2 completed!");
}

#[tokio::main]
async fn main() {
    fn1().await;
    fn2().await;
}
*/

/*
// Problem 2: Fix the code so that the two functions execute concurrently.
// Consider calling the functions inside spawned tasks.
// Solution:

use tokio::time::{sleep, Duration};

async fn fn1() {
    println!("Task 1 started!");
    sleep(Duration::from_secs(3)).await;
    println!("Task 1 completed!");
}

async fn fn2() {
    println!("Task 2 started!");
    sleep(Duration::from_secs(2)).await;
    println!("Task 2 completed!");
}

#[tokio::main]
async fn main() {
    let mut handles = vec![];
    let handle_1 = tokio::spawn(amove {
        fn1().await;
    });
    handles.push(handle_1);

    let handle_2 = tokio::spawn(async move {
        fn2().await;
    });
    handles.push(handle_2);

    for handle in handles {
        handle.await.unwrap();
    }
}
*/

/*
// -------------------------------------------
// 			    - Project: Web Scrapping
// -------------------------------------------

use std::sync::{mpsc, Arc, Mutex};
use std::thread;
use std::time::{Duration, Instant};
use ureq::{Agent, AgentBuilder};
fn main() -> Result<(), ureq::Error> {
    let webpages = vec![
        "https://gist.github.com/recluze/1d2989c7e345c8c3c542",
        "https://gist.github.com/recluze/a98aa1804884ca3b3ad3",
        "https://gist.github.com/recluze/5051735efe3fc189b90d",
        "https://gist.github.com/recluze/460157afc6a7492555bb",
        "https://gist.github.com/recluze/5051735efe3fc189b90d",
        "https://gist.github.com/recluze/c9bc4130af995c36176d",
        "https://gist.github.com/recluze/1d2989c7e345c8c3c542",
        "https://gist.github.com/recluze/a98aa1804884ca3b3ad3",
        "https://gist.github.com/recluze/5051735efe3fc189b90d",
        "https://gist.github.com/recluze/460157afc6a7492555bb",
        "https://gist.github.com/recluze/5051735efe3fc189b90d",
        "https://gist.github.com/recluze/c9bc4130af995c36176d",
        "https://gist.github.com/recluze/1d2989c7e345c8c3c542",
        "https://gist.github.com/recluze/a98aa1804884ca3b3ad3",
        "https://gist.github.com/recluze/5051735efe3fc189b90d",
        "https://gist.github.com/recluze/460157afc6a7492555bb",
        "https://gist.github.com/recluze/5051735efe3fc189b90d",
        "https://gist.github.com/recluze/c9bc4130af995c36176d",
    ];

    let agent = ureq::AgentBuilder::new().build();
    let now = Instant::now();

    for web_page in &webpages {
        let web_body = agent.get(web_page).call()?.into_string()?;
        println!(
            "Webpage {} has {} bytes, body contents {}",
            web_page,
            web_body.len(),
            web_body
        );
    }
    println!("Time taken wihtout Threads: {:.2?}", now.elapsed());

    let now = Instant::now();
    let agent = Arc::new(agent);
    let mut handles: Vec<thread::JoinHandle<Result<(), ureq::Error>>> = Vec::new();

    for web_page in webpages {
        let agent_thread = agent.clone();
        let t = thread::spawn(move || {
            let web_body = agent_thread.get(web_page).call()?.into_string()?;

            Ok(())
        });
        handles.push(t);
    }

    for handle in handles {
        handle.join().unwrap();
    }

    println!("Time taken using Threads: {:.2?}", now.elapsed());
    Ok(())
}
*/

// use regex::Regex;
// use std::error::Error;
// use ureq::Agent;

// fn main() -> Result<(), Box<dyn Error>> {
//     let url =
//         "https://www.bookdonga.com/mall/product_ebs_view.donga?product_seq=33389#view-tab-cont2"; // 웹페이지 URL을 여기에 입력하세요
//     let keyword = "방송일 오후 1시~"; // 찾고자 하는 특정한 글자를 여기에 입력하세요

//     let agent: Agent = ureq::AgentBuilder::new().build();
//     let response = agent.get(url).call()?.into_string()?;

//     let re = Regex::new(&format!(r"{}(.+?)\s", regex::escape(keyword)))?;
//     if let Some(captures) = re.captures(&response) {
//         if let Some(matched) = captures.get(1) {
//             println!("Found value: {}", matched.as_str());
//         } else {
//             println!("No match found.");
//         }
//     } else {
//         println!("No match found.");
//     }

//     Ok(())
// }
