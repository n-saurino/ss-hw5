#pragma once
#include <algorithm>
#include <deque>
#include <iostream>
#include <map>
#include <mutex>
#include <thread>
#include "ScopedThread.cpp"

/*
- Create a simple multithreaded trading engine simulation.
- Implement a shared order book where multiple threads can add buy/sell orders.
- Use mutexes to synchronize access to the order book.
- Ensure that orders are matched correctly and print the matched orders.
*/

/*

    Where should the locks occur?
    The assignment indicates we should use mutexes at the orderbook level (seems a little draconian)

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

std::mutex bid_mutex;
std::mutex ask_mutex;

enum class Side{
    Bid,
    Ask
};

typedef int64_t Price;
typedef int32_t Quantity;
typedef int32_t OrderId;

class Order{
private:
    static OrderId next_order_id;
    OrderId order_id_;
    Price price_;
    Quantity initial_quantity_;
    Quantity remaining_quantity_;
    Side side_;

public:
    Order(Price price, Quantity quantity, Side side): price_(price), initial_quantity_(quantity), 
                                                    remaining_quantity_(quantity), side_(side),
                                                    order_id_(++next_order_id){
    }

    ~Order(){

    }

    OrderId GetOrderId(){
        return order_id_;
    }

    void SetPrice(Price new_price){
        price_ = new_price;
    }

    Price GetPrice() const{
        return price_;
    }

    void SetRemainingQuantity(Quantity new_quantity){
        remaining_quantity_ = new_quantity;
    }

    Quantity GetRemainingQuantity() const{
        return remaining_quantity_;
    }

    Side GetSide() const{
        return side_;
    }

    std::string Print(){
        std::stringstream ss;
        ss << "OrderId: " << this->GetOrderId() << ", Price: " << this->GetPrice() << ", Quantity: " <<  this->GetRemainingQuantity();
        return ss.str();
    }

    bool IsFilled() const{
        return GetRemainingQuantity() == 0 ? true : false;
    }

};

typedef Order* POrder;
typedef std::deque<POrder> PriceLevel;
typedef std::map<Price, PriceLevel, std::greater<>> BidsAtPrice;
typedef std::map<Price, PriceLevel, std::less<>> AsksAtPrice;
typedef std::map<OrderId, POrder> Orders;

class Orderbook{
private:
    BidsAtPrice bids_;
    AsksAtPrice asks_;
    Orders orders_;

public:
    Orderbook(){

    }

    ~Orderbook(){

    }

    void AddOrder(const POrder& new_order){
        orders_[new_order->GetOrderId()] = new_order;
        if(new_order->GetSide() == Side::Bid){
            std::unique_lock<std::mutex> bid_lock(bid_mutex);
            bids_[new_order->GetPrice()].push_back(new_order);
        }else{
            std::unique_lock<std::mutex> ask_lock(ask_mutex);
            asks_[new_order->GetPrice()].push_back(new_order);
        }

        Match();
    }

    void CancelOrder(const POrder& cancel_order){
        if(orders_.find(cancel_order->GetOrderId()) == orders_.end()){
            throw std::logic_error("Order ID not found in Orders");
        }

        orders_.erase(cancel_order->GetOrderId());
        if(cancel_order->GetSide() == Side::Bid){
            PriceLevel& order_price_level = bids_[cancel_order->GetPrice()];
            auto it = std::find(order_price_level.begin(), order_price_level.end(), cancel_order);
            bids_[cancel_order->GetPrice()].erase(it);
            if(bids_[cancel_order->GetPrice()].empty()){
                bids_.erase(cancel_order->GetPrice());
            }
        }
    }

    bool CanMatch() const{
        // check if either Orderbook is empty
        if(bids_.empty() || asks_.empty()){
            return false;
        }

        // check if there is an order that crosses the book
        return bids_.begin()->first >= asks_.begin()->first ? true : false;
    }

    void Match(){
        // need to write a loop that checks and matches orders that are crossing the book
        // must also check that one side of the book is not empty each loop
        // don't actually take the locks yet

        std::unique_lock<std::mutex> bid_lock(bid_mutex, std::defer_lock);
        std::unique_lock<std::mutex> ask_lock(ask_mutex, std::defer_lock);
    
        // lock both unique_locks without deadlock
        std::lock(bid_lock, ask_lock);

        while(CanMatch()){ // takes care of the check that both sides of the book are full
            // need to fill both orders at the front of the price level vector 
            // for the minimum quantity between the two orders
            
            // first bid
            auto ptop_bid = bids_.begin()->second.front();
            
            // first ask
            auto ptop_ask = asks_.begin()->second.front();

            Quantity traded_quantity = std::min(ptop_bid->GetRemainingQuantity(), ptop_ask->GetRemainingQuantity());

            ptop_bid->SetRemainingQuantity(ptop_bid->GetRemainingQuantity() - traded_quantity);
            ptop_ask->SetRemainingQuantity(ptop_ask->GetRemainingQuantity() - traded_quantity);

            if(ptop_bid->IsFilled()){
                bids_.begin()->second.pop_front();
            }

            if(bids_.begin()->second.empty()){
                bids_.erase(ptop_bid->GetPrice());
            }

            if(ptop_ask->IsFilled()){
                asks_.begin()->second.pop_front();
            }

            if(asks_.begin()->second.empty()){
                asks_.erase(ptop_ask->GetPrice());
            }
        }
    }

    // Not needed for this exercise
    void ModifyOrder(){

    }

    void Print() const{
        // Print Bids
        std::cout << "-------------BIDS-------------" << std::endl;
        for(auto level_it = bids_.begin(); level_it != bids_.end(); ++level_it){
            std::cout << level_it->first << ": ";
            for(auto order_it = level_it->second.begin(); order_it != level_it->second.end(); ++order_it){
                std::cout << (*order_it)->Print() << " -> ";
            }
            std::cout << std::endl;
        }

        // Print Asks
        std::cout << "-------------ASKS-------------" << std::endl;
        for(auto level_it = asks_.begin(); level_it != asks_.end(); ++level_it){
            std::cout << level_it->first << ": ";
            for(auto order_it = level_it->second.begin(); order_it != level_it->second.end(); ++order_it){
                std::cout << (*order_it)->Print() << " -> ";
            }
            std::cout << std::endl;
        }
    }   
};


// Definition of the static member variable
int Order::next_order_id = 0;

int Ex5(){

    Order test_order(100, 10, Side::Bid);
    Order test_order2(100, 30, Side::Bid);
    Order test_order3(101, 20, Side::Ask);
    Order test_order4(102, 15, Side::Ask);
    Order test_order5(101, 100, Side::Bid);
    Order test_order6(99, 200, Side::Ask);

    Orderbook orderbook;
    {
        // must pass the orderbook by reference to the thread
        std::thread t1(&Orderbook::AddOrder, &orderbook, &test_order);
        ScopedThread s1(std::move(t1));

        std::thread t2(&Orderbook::AddOrder, &orderbook, &test_order2);
        ScopedThread s2(std::move(t2));

        std::thread t3(&Orderbook::AddOrder, &orderbook, &test_order3);
        ScopedThread s3(std::move(t3));
        
        std::thread t4(&Orderbook::AddOrder, &orderbook, &test_order4);
        ScopedThread s4(std::move(t4));

        std::thread t5(&Orderbook::AddOrder, &orderbook, &test_order5);
        ScopedThread s5(std::move(t5));

        std::thread t6(&Orderbook::AddOrder, &orderbook, &test_order6);
        ScopedThread s6(std::move(t6));
    }
    /*
    orderbook.AddOrder(&test_order);
    orderbook.AddOrder(&test_order2);
    orderbook.AddOrder(&test_order3);
    orderbook.AddOrder(&test_order4);
    orderbook.Print();
    orderbook.AddOrder(&test_order5);
    orderbook.Print();
    std::cout << std::endl << std::endl;

    orderbook.AddOrder(&test_order6);
    */

   orderbook.Print();


    return 0;
}