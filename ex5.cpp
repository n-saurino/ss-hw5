#pragma once
#include <iostream>
#include <map>
#include <mutex>
#include <thread>
#include <vector>
#include "ScopedThread.cpp"

/*
- Create a simple multithreaded trading engine simulation.
- Implement a shared order book where multiple threads can add buy/sell orders.
- Use mutexes to synchronize access to the order book.
- Ensure that orders are matched correctly and print the matched orders.
*/

/*
    
    struct Order:
        int64_t Price
        int32_t Quantity
        int32_t OrderId 
        enum Side: {Bid, Ask}

    Order* POrder
    vector<POrder> PriceLevel

    Class Orderbook:
    map<Price, vector<POrder>> orders_at_price_
    map<OrderId, POrder Orders> orders_

*/