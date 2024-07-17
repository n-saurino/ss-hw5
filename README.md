# ss-hw5
Introduction to Multithreading

## Objectives:

- Understand the fundamentals of multithreading in C++.
- Learn to use threads, mutexes, condition variables, and handle deadlocks.
- Gain hands-on experience in writing multithreaded applications.

## Reading:

1. **"C++ Concurrency in Action" by Anthony Williams**
    - Chapter 1: Introduction to concurrency in C++.
    - Chapter 2: Managing threads.
    - Chapter 3: Sharing data between threads.
2. **"The Art of Multiprocessor Programming" by Maurice Herlihy and Nir Shavit**
    - Chapter 1: Introduction.

## Tasks:

1. **Basic Multithreading**
    - Write a C++ program that starts multiple threads. Each thread should print a unique message along with the thread ID.
    - Use std::thread to create and manage threads.
2. **Mutexes and Data Sharing**
    - Extend the previous program to share a common resource (e.g., a counter) among the threads. Ensure that the access to this shared resource is synchronized using std::mutex.
    - Each thread should increment the counter a certain number of times (e.g., 1000 times).
3. **Condition Variables**
    - Implement a producer-consumer scenario using threads, mutexes, and condition variables.
    - One thread (the producer) should generate numbers and place them in a shared queue.
    - Another thread (the consumer) should read numbers from the queue and print them.
    - Use std::condition_variable to synchronize the producer and consumer.
4. **Deadlock Handling**
    - Write a program that deliberately creates a deadlock situation using two mutexes.
    - Identify and explain the cause of the deadlock in your code.
    - Modify the program to handle and avoid the deadlock.
5. **Multithreaded Trading Engine Simulation**
    - Create a simple multithreaded trading engine simulation.
    - Implement a shared order book where multiple threads can add buy/sell orders.
    - Use mutexes to synchronize access to the order book.
    - Ensure that orders are matched correctly and print the matched orders.
6. **Documentation and Best Practices**
    - Write a README file that explains the following:
        - The importance of multithreading in modern applications.
        - How mutexes and condition variables help in synchronizing threads.
        - Common pitfalls in multithreading, such as deadlocks, and how to avoid them.
    - Ensure your code follows best practices, such as proper indentation, meaningful variable names, and comments explaining the purpose of complex code sections.

## Submission:

- Create a GitHub repository for your assignment.
- Commit your code, including the README file, to the repository.
- Ensure your repository is private and share access with your instructor.

## Evaluation Criteria:

- **Correctness**: The programs should compile and run correctly, performing the specified tasks.
- **Multithreading Implementation**: Proper use of std::thread, std::mutex, and std::condition_variable.
- **Deadlock Handling**: Correct identification and handling of deadlock situations.
- **Code Quality**: Clean, readable, and well-documented code following best practices.
- **README File**: Clear and comprehensive explanation of multithreading concepts and challenges.

## Task 1 Example (Basic Multithreading):

```cpp
#include <iostream>
#include <thread>
#include <vector>

void printMessage(int threadID) {
    std::cout << "Thread ID: " << threadID << " is running." << std::endl;
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(printMessage, i);
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    return 0;
}
```

## **Task 2: Mutexes and Data Sharing**

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

std::mutex mtx;
int counter = 0;

void incrementCounter(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        std::lock_guard<std::mutex> lock(mtx);
        ++counter;
    }
}

int main() {
    const int numThreads = 5;
    const int iterations = 1000;
    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(incrementCounter, iterations);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "Final counter value: " << counter << std::endl;
    return 0;
}
```

## **Task 3: Condition Variables**

```cpp
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

std::queue<int> q;
std::mutex mtx;
std::condition_variable cv;
bool done = false;

void producer(int count) {
    for (int i = 0; i < count; ++i) {
        std::unique_lock<std::mutex> lock(mtx);
        q.push(i);
        cv.notify_one();
    }
    std::unique_lock<std::mutex> lock(mtx);
    done = true;
    cv.notify_all();
}

void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []{ return !q.empty() || done; });
        while (!q.empty()) {
            std::cout << "Consumer got: " << q.front() << std::endl;
            q.pop();
        }
        if (done) break;
    }
}

int main() {
    std::thread prod(producer, 10);
    std::thread cons(consumer);
    
    prod.join();
    cons.join();
    
    return 0;
}
```

## **Task 4: Deadlock Handling**

```cpp
#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx1;
std::mutex mtx2;

void thread1() {
    std::lock_guard<std::mutex> lock1(mtx1);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::lock_guard<std::mutex> lock2(mtx2);
    std::cout << "Thread 1 acquired both mutexes" << std::endl;
}

void thread2() {
    std::lock_guard<std::mutex> lock2(mtx2);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::lock_guard<std::mutex> lock1(mtx1);
    std::cout << "Thread 2 acquired both mutexes" << std::endl;
}

int main() {
    std::thread t1(thread1);
    std::thread t2(thread2);

    t1.join();
    t2.join();

    return 0;
}
```

- *Note*: The above code will cause a deadlock. To handle and avoid deadlocks, consider using std::lock or std::scoped_lock for multiple mutexes.

## **Task 5: Multithreaded Trading Engine Simulation**

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <map>

std::mutex orderBookMutex;
std::map<int, int> buyOrders;  // Price -> Quantity
std::map<int, int> sellOrders; // Price -> Quantity

void placeOrder(bool isBuyOrder, int price, int quantity) {
    std::lock_guard<std::mutex> lock(orderBookMutex);
    if (isBuyOrder) {
        buyOrders[price] += quantity;
    } else {
        sellOrders[price] += quantity;
    }
    std::cout << (isBuyOrder ? "Buy" : "Sell") << " order placed: Price " << price << ", Quantity " << quantity << std::endl;
}

void matchOrders() {
    std::lock_guard<std::mutex> lock(orderBookMutex);
    for (auto itBuy = buyOrders.begin(); itBuy != buyOrders.end(); ) {
        for (auto itSell = sellOrders.begin(); itSell != sellOrders.end(); ) {
            if (itBuy->first >= itSell->first) {
                int matchedQuantity = std::min(itBuy->second, itSell->second);
                std::cout << "Matched: Buy " << itBuy->first << " with Sell " << itSell->first << ", Quantity " << matchedQuantity << std::endl;
                itBuy->second -= matchedQuantity;
                itSell->second -= matchedQuantity;
                if (itBuy->second == 0) {
                    itBuy = buyOrders.erase(itBuy);
                } else {
                    ++itBuy;
                }
                if (itSell->second == 0) {
                    itSell = sellOrders.erase(itSell);
                } else {
                    ++itSell;
                }
            } else {
                ++itSell;
            }
        }
        if (itBuy == buyOrders.end()) break;
    }
}

int main() {
    std::thread traders[4];
    traders[0] = std::thread(placeOrder, true, 100, 10);
    traders[1] = std::thread(placeOrder, false, 90, 5);
    traders[2] = std::thread(placeOrder, true, 95, 2);
    traders[3] = std::thread(matchOrders);

    for (auto& trader : traders) {
```
## **Short Answers**
- How mutexes and condition variables help in synchronizing threads.
Mutexes and condition variables create a locking structure and inter-thread communication system. A mutex locks a resource so that modifications on that resource happen atomically. Condition variables allow threads to signal one another when there is work available to be done (i.e. a producer-consumer model sharing a queue with a mutex. The producer will notify the consumer who is waiting for the queue to be populated before performing some work on the queue (the shared resource)).

- Common pitfalls in multithreading, such as deadlocks, and how to avoid them.
Having two or more locks and requiring that a thread have both in order to proceed without paying attention to the ordering of the acquisition of the locks.