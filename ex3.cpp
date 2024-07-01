#pragma once
#include <chrono>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>
#include "ScopedThread.cpp"

/*
- Implement a producer-consumer scenario using threads, mutexes, and condition variables.
- One thread (the producer) should generate numbers and place them in a shared queue.
- Another thread (the consumer) should read numbers from the queue and print them.
- Use std::condition_variable to synchronize the producer and consumer.
*/

std::deque<int> shared_q;
std::condition_variable c;
std::mutex ex3_mutex;

void ThreadSafeEnque(int enque_val){
    // acquire the mutex
    {
        std::unique_lock<std::mutex> lgm(ex3_mutex);
        shared_q.push_back(enque_val);
        std::cout << "Produce: " << shared_q.back() << " / ";
    }

    c.notify_one();
}

void Produce(int val){
    // Loop to a certain value

    for(int i = 0; i < val; ++i){
        ThreadSafeEnque(i);
        // Bake in a delay
        std::this_thread::sleep_for(std::chrono::microseconds(rand()% 10 + 1));
    }
}

void ThreadSafeDeque(){
    // acquire the mutex
    std::unique_lock<std::mutex> lgm(ex3_mutex);
    c.wait(lgm);
    int val = shared_q.front();
    shared_q.pop_front();    
    std::cout << "Consume: " << val << std::endl;
}

void Consume(int val){
    for(int i = 0; i < val; ++i){
    ThreadSafeDeque();        
    // std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100 + 1));
    }

}


void Ex3(){

    int vals = 101;
    
    // create a producer thread
    std::thread producer(Produce, vals);
    ScopedThread scoped_producer(std::move(producer));
    
    // create a consumer thread
    std::thread consumer(Consume, vals);
    ScopedThread scoped_consumer(std::move(consumer));
   
}