#include <iostream>
#include <thread>

/*
Scoped thread class to use RAII for multithreading management
*/

class ScopedThread{
private:
    std::thread t_;
public:
    ScopedThread(std::thread t): t_(std::move(t)){
        if(!t_.joinable()){
            throw std::logic_error("No joinable thread passed in");
        }
    }

    ~ScopedThread(){
        t_.join();
    }
    ScopedThread(const ScopedThread&) = delete; // copy constructor deletion
    ScopedThread& operator=(const ScopedThread&) = delete; // assignment operator deletion
};