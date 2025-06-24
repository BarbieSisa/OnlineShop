#pragma once

#include <iostream>
#include "MyString.h"
#include "MyVector.hpp"
class Item {
private:
    int id = 0;
    MyString name;
    double price = 0.0;
    int quantity = 0;
    MyString description;
    double rating = 0.0;
    bool available = true;

    static MyVector<Item> items;
    static int nextId;

public:
    Item() = default;

    Item(const MyString& name, double price, int quantity,
        const MyString& description, double rating);

    // Getters
    int getId() const;
    MyString getName() const;
    double getPrice() const;
    int getQuantity() const;
    MyString getDescription() const;
    double getRating() const;
    bool getAvailable() const;
   static int toInt(const char* str);
   static double toDouble(const char* str);
    // Setters
    void setId(int newId);
    void setName(const MyString& name);
    void setPrice(double price);
    void setQuantity(int newQuantity);
    void setAvailable(bool available);
    void setRating(double newRating);

    // CRUD operations
    static void create();
    static void removeById(int idToRemove);
    static void displayByPriceAsc();
    static void displayByAlphabetical();
    static void displayByPriceDesc();
    static void displayByRating();
    static Item read(int id);
    static void remove(int id);
    static void listProducts();
    static void displayAllProducts();
    static void viewProductById(int searchId);

    // Utilities
    static int findIndexById(int id);
    static void updateQuantity(int productId, int newQuantity);
    static void updateRating(int productId, double newRating);

    // If needed later:
    // static void loadFromFile();
    // static void setNextId(int value);
};
