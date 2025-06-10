#pragma once
#include <string>
#include <vector>
#include <iostream>

class Item {
    int id = 0;
    std::string name;
    double price = 0.0;
    int quantity = 0;
    std::string description;
    double rating = 0.0;
    bool available = true;

    static std::vector<Item> items;
    static int nextId;

   

public:
    Item() = default;
    std::string getDescription() const;
    double getRating() const;

    Item(const std::string& name, double price, int quantity,
        const std::string& description, double rating);

    // CRUD interface
    static void create();                          // static to avoid using default object
    static Item read(int id);
    static void update(int id);
    static void remove(int id);
    static void listAllItems();

    // Utility
    void printDetails() const;
    bool getAvailable() const;
    double getPrice() const;
    int getQuantity() const;
    int getId() const;
    void setId(int newId);
    void setQuantity(int newQuantity);
    void setName(const std::string& name);
    void setPrice(double price);
    static int findIndexById(int id);
    void setAvailable(bool available);
    static void updateQuantity(int, int);
      const std::string& getName() const;
};
