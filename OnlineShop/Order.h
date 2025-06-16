// Order.h
#pragma once

#include "Item.h"
#include <vector>
#include <string>
#include <map>

enum class OrderStatus {
    Pending,
    Confirmed,
    Shipped,
    Delivered
};

std::string statusToString(OrderStatus status);

class Order {
    int id;
    std::vector<std::pair<Item, int>> items; // Item + quantity
    OrderStatus status;

    static std::vector<Order> orders;
    static int nextId;
    static int findIndexById(int id);


    std::string clientEGN;


public:
    Order();

    void addItem(const Item& item);
    double getTotalPrice() const;
    double getRewardPoints() const;
   static void saveToFile();
    static void updateOrderStatusById(int id);
    void setStatus(OrderStatus newStatus);
    void display() const;

   static void displayRejectedOrders();
   static void displayRefundedOrders();
  static void rejectRefundOrderById(int id, std::string reason);
   static void displayRefundedOrdersUsers(const std::string& currentEGN);
    static void refundOrderById(int id);
    static void rejectOrderById(int id, std::string reason);
   static void displayPending();

   static void displayShipped();

  static void displayDelivered();

  static void displayConfirmed();

  static void UpdatePointsWhenConfirmed(int orderId);

   static void displayUser(std::string eng);

   static void displayUserSum(std::string eng);

  static void displayUserDelivered(std::string eng);
    
    int getId() const;
    OrderStatus getStatus() const;
    const std::vector<std::pair<Item, int>>& getItems() const;
    static void addItemToOrder(std::vector<Item>& items);
   static void removeItemFromOrder(std::vector<Item>& items);
 static void applyDiscount(const std::string& egn, const std::vector<Item>& items, bool& discountApplied);
  //static bool applyDiscount(const std::string& egn, std::vector<Item>& items);
   static void viewCart(const std::vector<Item>& items, bool discountApplied);
   static void checkoutOrder(const std::vector<Item>& items, const std::string& clientEGN);
  static void removeOrderById(int idToRemove);
 static void approveRefund(int orderIdToMove);
 // static void applyDiscount(const std::string& egn, const std::vector<Item>& items, bool& discountApplied);

  static void removeDiscount(std::string egn, bool& discountApplied);
   
  //void loadOrdersFromFile();
   // static void createOrder();
    static void displayAllOrdersFromFile();
    void setClientEGN(const std::string& egn);
  //  static Order read(int id);

    void setId(int newId);
    static void update(int id);
  // static void remove(int id);
    static void listAllOrders();
 // static void saveToFile();
    static void loadFromFile();
   static void loadFromFileRefunds();
};
