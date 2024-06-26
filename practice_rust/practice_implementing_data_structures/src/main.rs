/*
// -------------------------------------------
// 		Link List (Part 1)
// -------------------------------------------

#[derive(Debug)]
struct Linklist {
    head: pointer,
}

#[derive(Debug)]
struct Node {
    element: i32,
    next: pointer,
}

type pointer = Option<Box<Node>>;
fn main() {
    // let list = Node {
    //     element: 1,
    //     next: None,
    // };

    // let list = Node {
    //     element: 1,
    //     next: Some(Box::new(Node {
    //         element: 2,
    //         next: None,
    //     })),
    // };

    // let list = Linklist {
    //     head: Some(Node {
    //         element: 1,
    //         next: None,
    //     }),
    // };

    // let list = Linklist {
    //     head: Some(Node {
    //         element: 1,
    //         next: Some(Box::new(Node {
    //             element: 2,
    //             next: Some(Box::new(Node {
    //                 element: 3,
    //                 next: None,
    //             })),
    //         })),
    //     }),
    // };

    // let list = Linklist { head: None };

    let list = Linklist {
        head: Some(Box::new(Node {
            element: 100,
            next: Some(Box::new(Node {
                element: 200,
                next: None,
            })),
        })),
    };

    println!("{:?}", &list.head);
}
*/

/*
// -------------------------------------------
// 		Link List (Part 2)
// -------------------------------------------

#[derive(Debug)]
struct Linklist {
    head: pointer,
}

#[derive(Debug)]
struct Node {
    element: i32,
    next: pointer,
}
type pointer = Option<Box<Node>>;

impl Linklist {
    fn new() -> Linklist {
        Linklist { head: None }
    }

    fn add(&mut self, element: i32) {
        // match self.head {
        //     None => {
        //         let new_node = Some(Box::new(Node {
        //             element: element,
        //             next: None,
        //         }));
        //         self.head = new_node;
        //     }
        //     Some(previous_head) => {
        //         let new_node = Some(Box::new(Node {
        //             element: element,
        //             next: Some(previous_head),
        //         }));
        //         self.head = new_node;
        //     }
        // }

        // fn take<T>(dest: &mut T) -> T
        let previous_head = self.head.take();
        let new_head = Some(Box::new(Node {
            element: element,
            next: previous_head,
        }));
        self.head = new_head;
    }

    fn remove(&mut self) -> Option<i32> {
        match self.head.take() {
            Some(previous_head) => {
                self.head = previous_head.next;
                Some(previous_head.element)
            }
            None => None,
        }
    }

    fn print(&self) {
        let mut list_traversal = &self.head;
        while !list_traversal.is_none() {
            println!("{:?}", list_traversal.as_ref().unwrap().element);
            list_traversal = &list_traversal.as_ref().unwrap().next;
        }
    }
}
fn main() {
    let mut list = Linklist::new();
    list.add(5);
    list.add(7);
    list.add(10);
    list.add(15);
    list.add(20);

    //println!("List: {:?}", list);
    list.print();
    println!("{}", list.remove().unwrap());
}
*/

/*
//Problem 1: Implement a peek method on the Linklist.
// The signature of the function is given in the code below.
// This method will return the head value if it exist.

#[derive(Debug)]
struct Linklist {
    head: pointer,
}

#[derive(Debug)]
struct Node {
    element: i32,
    next: pointer,
}
type pointer = Option<Box<Node>>;

impl Linklist {
    fn new() -> Linklist {
        Linklist { head: None }
    }

    fn add(&mut self, element: i32) {
        let previous_head = self.head.take();
        let new_head = Some(Box::new(Node {
            element: element,
            next: previous_head,
        }));
        self.head = new_head;
    }

    fn remove(&mut self) -> Option<i32> {
        match self.head.take() {
            Some(previous_head) => {
                self.head = previous_head.next;
                Some(previous_head.element)
            }
            None => None,
        }
    }

    fn peek(&self) -> Option<i32> {
        match &self.head {
            Some(head) => {
                let value = head.element;
                Some(value)
            }
            None => None,
        }
    }

    fn print(&self) {
        let mut list_traversal = &self.head;
        while !list_traversal.is_none() {
            println!("{:?}", list_traversal.as_ref().unwrap().element);
            list_traversal = &list_traversal.as_ref().unwrap().next;
        }
    }
}
fn main() {
    let mut list = Linklist::new();
    list.add(5);
    list.add(7);
    list.add(10);
    list.add(15);
    list.add(20);

    println!("{:?}", list.peek());
}
*/

//Problem 2: We want to change the linked list implementation by making the element part of
// the node as generic rather then concrete i32. Make approperiate changes to the code below.
// For printing, T should have the trait bound of  std::fmt::Debug and
// for the peek to work, T must also have the trait bound of std::marker::Copy

#[derive(Debug)]
struct Linklist<T> {
    head: pointer<T>,
}

#[derive(Debug)]
struct Node<T> {
    element: T,
    next: pointer<T>,
}
type pointer<T> = Option<Box<Node<T>>>;

impl<T: std::fmt::Debug> Linklist<T> {
    fn new() -> Linklist<T> {
        Linklist { head: None }
    }

    fn add(&mut self, element: T) {
        let previous_head = self.head.take();
        let new_head = Some(Box::new(Node {
            element: element,
            next: previous_head,
        }));
        self.head = new_head;
    }

    fn remove(&mut self) -> Option<T> {
        match self.head.take() {
            Some(previous_head) => {
                self.head = previous_head.next;
                Some(previous_head.element)
            }
            None => None,
        }
    }

    //  fn peek(&self) -> Option<T> {
    //      match &self.head {
    //          Some(head) => {
    //              let value = head.element;
    //              Some(value)
    //          }
    //          None => None,
    //      }
    //  }

    fn print(&self) {
        let mut list_traversal = &self.head;
        while !list_traversal.is_none() {
            println!("{:?}", list_traversal.as_ref().unwrap().element);
            list_traversal = &list_traversal.as_ref().unwrap().next;
        }
    }
}
fn main() {
    let mut list = Linklist::new();
    list.add(5);
    list.add(7);
    list.add(10);
    list.add(15);
    list.add(20);

    //println!("{:?}", list.peek());
}
