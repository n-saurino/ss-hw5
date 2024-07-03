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


class Order{
private:
    static OrderId order_id_;
    Price price_;
    Quantity initial_quantity_;
    Quantity remaining_quantity_;
    Side side_;

public:
    Order(Price price, Quantity quantity, Side side): price_(price), 
                                                                        initial_quantity_(quantity), remaining_quantity_(quantity),
                                                                        side_(side){
        order_id_++;
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

    void AddOrder(const POrder& new_order){
        orders_[new_order->GetOrderId()] = new_order;
        if(new_order->GetSide() == Side::Bid){
            bids_[new_order->GetPrice()].push_back(new_order);
        }else{
            asks_[new_order->GetPrice()].push_back(new_order);
        }
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

    void ModifyOrder(){

    }

    void Print() const{
        // Print Bids
        for(auto level_it = bids_.begin(); level_it != bids_.end(); ++level_it){
            std::cout << level_it->first << ": ";
            for(auto order_it = level_it->second.begin(); order_it != level_it->second.end(); ++order_it){
                std::cout << (*order_it)->Print() << " -> ";
            }
            std::cout << std::endl;
        }

        // Print Asks
        for(auto level_it = asks_.begin(); level_it != asks_.end(); ++level_it){

        }
    }   
};

// Definition of the static member variable
int Order::order_id_ = 0;

int Ex5(){
    Order test_order(100, 10, Side::Bid);
    Order test_order2(100, 30, Side::Bid);
    std::cout << test_order.Print() << std::endl;

    Orderbook orderbook;
    orderbook.AddOrder(&test_order);
    orderbook.AddOrder(&test_order2);
    orderbook.Print();
    return 0;
}