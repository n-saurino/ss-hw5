#pragma once
#include <mutex>
#include "ScopedThread.cpp"
/*
- Extend the previous program to share a common resource (e.g., a counter) 
- among the threads. Ensure that the access to this shared resource is 
- synchronized using std::mutex.
- Each thread should increment the counter a certain number of times (e.g., 1000 times).
*/

volatile int global_counter;
std::mutex m;

void Increment(int increment_val){
    std::lock_guard<std::mutex> lg_m(m);
    for(int i = 0; i < increment_val; ++i){
        global_counter++;
    }
}

void Ex2(){
    global_counter = 0;
    std::thread t1(Increment, 1000);
    std::thread t2(Increment, 1000);

    t1.join();
    t2.join();

    std::cout << global_counter << std::endl;
}