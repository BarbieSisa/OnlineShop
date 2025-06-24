#pragma once

#include "Item.h"
#include "MyVector.hpp"
#include "MyString.h"

enum class OrderStatus {
    Pending,
    Confirmed,
    Shipped,
    Delivered
};

MyString statusToString(OrderStatus status);

class Order {
    int id;
    MyVector<Item> items;
    MyVector<int> quantities;
    OrderStatus status;
    MyString clientEGN;

    static MyVector<Order> orders;
    static int nextId;
    static int findIndexById(int id);

    

public:
    Order();
    double getTotalPrice() const;
    double getRewardPoints() const;

    static void saveToFile();
    static void updateOrderStatusById(int id);
    void setStatus(OrderStatus newStatus);
    void display() const;

    static void displayRejectedOrders();
    static void displayRefundedOrders();
    static void rejectRefundOrderById(int id, const MyString& reason);
    static void displayRefundedOrdersUsers(const MyString& currentEGN);
    static void refundOrderById(int id);
    static void rejectOrderById(int id, const MyString& reason);
    static void displayPending();
    static void displayShipped();
    static void displayDelivered();
    static void displayConfirmed();
    static void UpdatePointsWhenConfirmed(int orderId);
    static void displayUser(const MyString& egn);
    static void displayUserSum(const MyString& egn);
    static void displayUserDelivered(const MyString& egn);
    static void removeOrderById(int idToRemove);
    static void approveRefund(int orderIdToMove);
    static void removeDiscount(const MyString egn, bool& discountApplied);
    static void displayAllOrdersFromFile();
    MyVector<Item>& getItemsRef();         
    MyVector<int>& getQuantitiesRef();
    void setClientEGN(const MyString& egn);
    void setId(int newId);
    MyString getClientEGN() const;
    static void listAllOrders();
    static void loadFromFile();
    static void loadFromFileRefunds();
    void addItem(const Item& item, int quantity = 1);
    int getId() const;
    OrderStatus getStatus() const;
   const MyVector<Item>& getItems() const;
   const MyVector<int>& getQuantities() const;
 
    void setItems(const MyVector<Item>& newItems);
    void setQuantities(const MyVector<int>& newQuantities);
    static void addItemToOrder(MyVector<Item>& , MyVector<int>& );
    static void removeItemFromOrder(MyVector<Item>& items, MyVector<int>& quantities);
    static void applyDiscount(const MyString& egn, const MyVector<Item>& items, bool& discountApplied);
    static void viewCart(const MyVector<Item>& items, double discountApplied);
    static void checkoutOrder(const MyVector<Item>& items, const MyString& clientEGN);
};
