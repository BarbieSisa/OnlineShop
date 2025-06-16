#pragma once

#include "User.h"
#include "UserCrud.h"
#include "Order.h"
#include "Admin.h"
#include <vector>
#include <string>

class Client : public User, public UserCrud
{
    double balance = 0.0;
    std::vector<Order> orders;
    static std::vector<Client> clients;
    std::string clientEGN;
    

public:
    Client() = default;
    size_t points = 0;
    static int findIndexByEGN(const std::string& egn);
    static Client& getClientByIndex(int index) {
        return clients.at(index);
    }
    Client(const char* name, const char* egn, const char* password, double balance);

 //  static void loadAllClientsFromFile();

    void setPoints(int newPoints);

   

  static void updateBalanceForOrder(const std::string& eng,double amount);
 static void updateBalance(const std::string& egn, double amount);
 static void updatePoints(const std::string& egn, double newPoints);
    //static void updatePoints(std::string eng,double newPoints);
    static void saveAllClientsToFile();
    std::string loginClientByUsernameAndPassword(const std::string& username, const std::string& password);
   static void create() ;
  static void approveCheck(const std::string& code, const std::string& egn);
   static void loadAllClientsFromFile();

    double getBalance() const { return balance; }
    size_t getPoints() const { return points; }
    void checkBalance() const;
    void redeem(const std::string& code);
    std::string getEGN() const;
            void listProducts() const;
    void filterByRating() const;
    void filterByPrice(bool ascending = true) const;
    void filterByAlphabeticalOrder() const;
    void viewProduct(int productId) const;

   void addToCart(int productId, int quantity);
    void removeFromCart(const std::string& productName, int quantity);
    static void applyDiscount(const std::string& egn, const std::vector<Item>& items, double& discountApplied);
   static void removeDiscount(std::string egn, bool& discountApplied);
   static void viewCart(const std::vector<Item>& items, bool discountApplied) ;
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
    void loadClientOrders(const std::string& egn);
};
