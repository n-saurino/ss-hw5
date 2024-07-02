#pragma once
#include <thread>
#include <mutex>
#include <iomanip>
#include <iostream>
#include <chrono>
#include "ScopedThread.cpp"

/*
    - Write a program that deliberately creates a deadlock situation using two mutexes.
    - Identify and explain the cause of the deadlock in your code.
    - Modify the program to handle and avoid the deadlock.
*/

/*
    Create a program where we need to grab two mutexes in order to complete a job.
    Then have the threads grab the mutexes in opposite order.
    Solution is to then force them to grab the mutexes in the same order.
    Write two functions that grab the mutexes in opposite orders.
    Instantiate the two threads with those functions.

    Can also instead use a scoped lock for both cases to acquire the mutexes 
    at the same time
*/

std::mutex m1;
std::mutex m2;

void PrintForwardOrder(){
    // solution is to use lock() or scoped_lock()
    lock(m1, m2);
    std::lock_guard<std::mutex> guard1(m1, std::adopt_lock);
    std::lock_guard<std::mutex> guard2(m2, std::adopt_lock);
    std::cout << "Printing from Forward Order!" << std::endl;
}


void PrintReverseOrder(){
    // solution is to use lock() or scoped_lock()
    lock(m1, m2);
    std::lock_guard<std::mutex> guard2(m2, std::adopt_lock);
    std::lock_guard<std::mutex> guard1(m1, std::adopt_lock);
    std::cout << "Printing from Reverse Order!" << std::endl;
}

void ExecuteThread(int flag){
    for(int i = 0; i < 100; i++){
        if(flag){
            PrintForwardOrder();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }else{
            PrintReverseOrder();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    
}

void Ex4(){
    std::thread t1(ExecuteThread, 1);
    ScopedThread scoped_t1(std::move(t1));

    // creates deadlock by acquiring the locks in opposite orders.
    std::thread t2(ExecuteThread, 0);
    ScopedThread scoped_t2(std::move(t2));

}