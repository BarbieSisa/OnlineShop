#pragma once

#include "User.h"
#include "Order.h"
#include "Admin.h"
#include "MyString.h"
#include "MyVector.hpp"

class Client : public User
{
    double balance = 0.0;
    MyVector<Order> orders;
  
    MyString clientEGN;

public:
    static MyVector<Client> clients;
    Client() = default;
    size_t points = 0;

    Client(const char* name, const char* egn, const char* password, double balance);

    static int findIndexByEGN(const MyString& egn);
    static Client& getClientByIndex(int index) {
        return clients[index];
    }

    void setPoints(int newPoints);
    double getBalance() const { return balance; }
    size_t getPoints() const { return points; }
    MyString getEGN() const;

    void checkBalance() const;
    void setBalance(double newBalance);

    static void updateBalanceForOrder(const MyString& egn, double amount);
    static void updateBalance(const MyString& egn, double amount);
    static void updatePoints(const MyString& egn, int newPoints);

    static void saveAllClientsToFile();
    static void loadAllClientsFromFile();

    MyString loginClientByUsernameAndPassword(const MyString& username, const MyString& password);

    static void create();
    static void approveCheck(const MyString& code, const MyString& egn);

    void listProducts() const;
    void filterByRating() const;
    void filterByPrice(bool ascending = true) const;
    void filterByAlphabeticalOrder() const;
    void viewProduct(int productId) const;

    void addToCart(int productId, int quantity);
    void removeFromCart(const MyString& productName, int quantity);
    static void applyDiscount(const MyString& egn, const MyVector<Item>& items, double& discountApplied);
    static void removeDiscount(const MyString& egn, bool& discountApplied);
    static void viewCart(const MyVector<Item>& items, bool discountApplied);
    void checkout();

    void listOrders() const;
    void confirmOrder(size_t index);
    void orderHistory() const;
    void rate(int productId, int rating);
    void requestRefund();
    void refundedOrders() const;

    void logout() const override;
    void help() const override;
    void viewProfile() const override;

    void addOrder(const Order& order);
    void createOrderAsClient();
    void loadClientOrders(const MyString& egn);
    static int toInt(const char* str);
    static double toDouble(const char* str);
};
