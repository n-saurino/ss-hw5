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
    OrderId order_id_;
    Price price_;
    Quantity initial_quantity_;
    Quantity remaining_quantity_;
    Side side_;

public:
    Order(OrderId order_id, Price price, Quantity quantity, Side side): order_id_(order_id), price_(price), 
                                                                        initial_quantity_(quantity), remaining_quantity_(quantity),
                                                                        side_(side){
                                                    
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
};

int Ex5(){

    return 0;
}