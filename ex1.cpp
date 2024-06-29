#include <iostream>
#include <string>
#include "ScopedThread.cpp"
/*
EXERCISE 1:
- Write a C++ program that starts multiple threads. 
- Each thread should print a unique message along with the thread ID.
- Use std::thread to create and manage threads.
*/

void Print(const std::string& message){
    // must compose your string first using a stringstream object 
    // otherwise there will be a race on the << operator resulting in mixing of outputs
    // see https://stackoverflow.com/questions/14718124/how-to-easily-make-stdcout-thread-safe
    std::stringstream msg;
    msg << message << std::endl;
    std::cout << msg.str();
}

int Ex1(){

    std::thread t1(Print, std::string("Hello from T1!"));
    ScopedThread s1(std::move(t1));

    std::thread t2(Print, std::string("Hello from T2!"));
    ScopedThread s2(std::move(t2));

    std::thread t3(Print, std::string("Hello from T3!"));
    ScopedThread s3(std::move(t3));
 
    Print("Hello from OG thread");
    return 0;
}