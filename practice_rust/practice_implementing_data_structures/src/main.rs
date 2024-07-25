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

/*
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
*/

/*

// -------------------------------------------
// 		Doubly Link List (Part 1)
// -------------------------------------------

use std::{cell::RefCell, rc::Rc};
#[derive(Debug)]
struct Doubly_Linklist {
    head: pointer,
    tail: pointer,
}

#[derive(Debug)]
struct Node {
    element: i32,
    next: pointer,
    prev: pointer,
}

type pointer = Option<Rc<RefCell<Node>>>;

impl Doubly_Linklist {
    fn new() -> Self {
        Doubly_Linklist {
            head: None,
            tail: None,
        }
    }

    fn add(&mut self, element: i32) {
        let new_head = Node::new(element);

        match self.head.take() {
            Some(old_head) => {
                old_head.borrow_mut().prev = Some(new_head.clone());
                new_head.borrow_mut().next = Some(old_head.clone());
                self.head = Some(new_head);
            }

            None => {
                self.tail = Some(new_head.clone());
                self.head = Some(new_head);
            }
        }
    }
}

impl Node {
    fn new(element: i32) -> Rc<RefCell<Node>> {
        Rc::new(RefCell::new(Node {
            element: element,
            next: None,
            prev: None,
        }))
    }
}
fn main() {}

*/

/*
// -------------------------------------------
// 		Doubly Link List (Part 2)
// -------------------------------------------

use std::{cell::RefCell, rc::Rc};
#[derive(Debug)]
struct Doubly_Linklist {
    head: pointer,
    tail: pointer,
}

#[derive(Debug)]
struct Node {
    element: i32,
    next: pointer,
    prev: pointer,
}

type pointer = Option<Rc<RefCell<Node>>>;

impl Doubly_Linklist {
    fn new() -> Self {
        Doubly_Linklist {
            head: None,
            tail: None,
        }
    }

    fn add(&mut self, element: i32) {
        let new_head = Node::new(element);

        match self.head.take() {
            Some(old_head) => {
                old_head.borrow_mut().prev = Some(new_head.clone());
                new_head.borrow_mut().next = Some(old_head.clone());
                self.head = Some(new_head);
            }

            None => {
                self.tail = Some(new_head.clone());
                self.head = Some(new_head);
            }
        }
    }

    // Case: 1
    // -----------------------
    //         Head        Tail
    // None <-- 1 --> 2 --> 3 --> None
    // None     1 <-- 2 <-- 3     None
    // -----------------------

    // Case: 1 (After Removal)
    // -----------------------
    //       Head  Tail
    // None <-- 2 --> 3 --> None
    // None     2 <-- 3     None
    // -----------------------

    // Case: 2
    // -----------------------
    //       Head
    //       Tail
    // None <-- 1 --> None
    // -----------------------

    // Case: 2 (After Removal)
    // -----------------------
    //       Head = None
    //       Tail = None
    // -----------------------

    fn remove(&mut self) -> Option<i32> {
        if self.head.is_none() {
            println!("List is empty so we can not remove");
            None
        } else {
            let removed_val = self.head.as_ref().unwrap().borrow().element;
            self.head
                .take()
                .map(|old_head| match old_head.borrow_mut().next.take() {
                    Some(new_head) => {
                        new_head.borrow_mut().prev = None;
                        self.head = Some(new_head);
                        self.head.clone()
                    }
                    None => {
                        self.tail = None;
                        println!("List is empty after removal");
                        None
                    }
                });
            Some(removed_val)
        }
    }

    fn print(&self) {
        let mut traversal = self.head.clone();
        while !traversal.is_none() {
            println!("{}", traversal.as_ref().unwrap().borrow().element);
            traversal = traversal.unwrap().borrow().next.clone();
        }
    }
}

impl Node {
    fn new(element: i32) -> Rc<RefCell<Node>> {
        Rc::new(RefCell::new(Node {
            element: element,
            next: None,
            prev: None,
        }))
    }
}
fn main() {
    let mut list1 = Doubly_Linklist::new();

    list1.add(30);
    list1.add(32);
    list1.add(34);
    list1.add(36);
    list1.print();

    list1.remove();
    println!("After Removal");
    list1.print();
}
*/

//Problem 1: Add the method add_back() to the implementation of doubly linked list below.
// The method should add the element at the back or at the end of the linked list.

use std::{cell::RefCell, rc::Rc};
#[derive(Debug)]
struct Doubly_Linklist {
    head: pointer,
    tail: pointer,
}

#[derive(Debug)]
struct Node {
    element: i32,
    next: pointer,
    prev: pointer,
}

type pointer = Option<Rc<RefCell<Node>>>;

impl Doubly_Linklist {
    fn new() -> Self {
        Doubly_Linklist {
            head: None,
            tail: None,
        }
    }

    fn add(&mut self, element: i32) {
        let new_head = Node::new(element);

        match self.head.take() {
            Some(old_head) => {
                old_head.borrow_mut().prev = Some(new_head.clone());
                new_head.borrow_mut().next = Some(old_head.clone());
                self.head = Some(new_head);
            }

            None => {
                self.tail = Some(new_head.clone());
                self.head = Some(new_head);
            }
        }
    }

    fn add_back(&mut self, element: i32) {
        let new_tail = Node::new(element);

        match self.tail.take() {
            Some(old_tail) => {
                println!("old_tail: {}", old_tail.borrow().element);
                new_tail.borrow_mut().next = None;
                new_tail.borrow_mut().prev = Some(old_tail.clone());
                self.tail = Some(new_tail);
            }

            None => {
                self.tail = Some(new_tail.clone());
            }
        }
    }

    fn remove(&mut self) -> Option<i32> {
        if self.head.is_none() {
            println!("List is empty so we can not remove");
            None
        } else {
            let removed_val = self.head.as_ref().unwrap().borrow().element;
            self.head
                .take()
                .map(|old_head| match old_head.borrow_mut().next.take() {
                    Some(new_head) => {
                        new_head.borrow_mut().prev = None;
                        self.head = Some(new_head);
                        self.head.clone()
                    }
                    None => {
                        self.tail = None;
                        println!("List is empty after removal");
                        None
                    }
                });
            Some(removed_val)
        }
    }

    fn print(&self) {
        let mut traversal = self.head.clone();
        while !traversal.is_none() {
            println!("{}", traversal.as_ref().unwrap().borrow().element);
            traversal = traversal.unwrap().borrow().next.clone();
        }
    }
}

impl Node {
    fn new(element: i32) -> Rc<RefCell<Node>> {
        Rc::new(RefCell::new(Node {
            element: element,
            next: None,
            prev: None,
        }))
    }
}
fn main() {
    let mut list1 = Doubly_Linklist::new();

    list1.add(30);
    list1.add(32);
    list1.add(34);
    list1.add(36);
    list1.add_back(1);
    list1.add_back(2);
    list1.print();
}
