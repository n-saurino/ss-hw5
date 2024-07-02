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
    BLUEPRINT

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

        AddOrder()
        CancelOrder()
        ModifyOrder()

*/

enum class Side{
    Bid,
    Ask
};

typedef int64_t Price;
typedef int32_t Quantity;
typedef int32_t OrderId;

struct Order{
    OrderId order_id_;
    Price price_;
    Quantity quantity_;
    Side side_;
};

typedef Order* POrder;
typedef std::vector<POrder> PriceLevel;
typedef std::map<Price, PriceLevel> OrdersAtPrice;
typedef std::map<OrderId, POrder> Orders;

class Orderbook{
private:
    OrdersAtPrice bids_;
    OrdersAtPrice asks_;
    Orders orders_;

public:
    Orderbook(){

    }

    ~Orderbook(){

    }
};

int Ex5(){

    return 0;
}