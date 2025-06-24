#include "BusinessAccount.h"
#include <iostream>
#include<fstream>
#include<iostream>
#include "MyString.h"
#include "MyVector.hpp"
#include "Order.h"
#include "Client.h"
#include "Item.h"
#include "User.h"
BusinessAccount::BusinessAccount(const MyString& name, const MyString& egn, const MyString& password)
    : User(name.c_str(), egn.c_str(), password.c_str()) {
}
void BusinessAccount::setSoldItem(const Item& item, int count) {
    soldItem.set(item, count);
}
const BusinessSoldItem& BusinessAccount::getSoldItem() const {
    return soldItem;
}
MyString BusinessAccount::loginBusinessByUsernameAndPassword(const MyString& username, const MyString& password) {
    std::ifstream inFile("BusinessAccount.txt");
    if (!inFile.is_open()) {
        std::cout << "❌ Cannot open BusinessAccount.txt\n";
        return "invalid";
    }

    char name[128];
    char egn[32];
    char storedPassword[128];
    char itemName[128];
    int itemId, availability;
    double price;

    while (
        inFile.getline(name, 128) &&
        inFile.getline(egn, 32) &&
        inFile.getline(storedPassword, 128) &&
        (inFile >> itemId) &&
        inFile.ignore() &&
        inFile.getline(itemName, 128) &&
        (inFile >> price) &&
        inFile.ignore() &&
        (inFile >> availability) &&
        inFile.ignore()
        ) {
        MyString nameStr(name);
        MyString egnStr(egn);
        MyString passStr(storedPassword);

        if (username == nameStr && password == passStr) {
            return egnStr;  // 🟢 успех
        }
    }

    inFile.close();
    return "invalid";  // 🔴 неуспех
}
void BusinessAccount::viewSoldItems() const {
    cout << "Sold Item Info:" << endl;
    cout << "Quantity: " << soldItem.quantity << endl;
    cout << "Item Name: " << soldItem.item.getName() << endl;
}
void BusinessAccount::viewProfile() const {
    cout << "Business Profile:\n";
    cout << "Name: " << getName() << endl;
    cout << "EGN: " << getEGN() << endl;
    viewSoldItems();
}
void BusinessAccount::help() const {
    cout << "Business Commands:\n"
        << " - add-item\n"
        << " - remove-item\n"
        << " - list-orders\n"
        << " - approve-order\n"
        << " - reject-order\n"
        << " - view-revenue\n";
}
void BusinessAccount::create() {
    char name[64], egn[16], password[64];
    std::cout << "Enter business name: ";
    std::cin.getline(name, 64);
    std::cout << "Enter EGN: ";
    std::cin.getline(egn, 16);
    std::cout << "Enter password: ";
    std::cin.getline(password, 64);

    std::ifstream inFile("Items.txt");
    if (!inFile.is_open()) {
        std::cerr << "❌ Cannot open Items.txt\n";
        return;
    }

    const int MAX_ITEMS = 100;
    Item availableItems[MAX_ITEMS];
    int itemCount = 0;

    while (!inFile.eof() && itemCount < MAX_ITEMS) {
        char buffer[256];
        inFile.getline(buffer, 256);
        int id = atoi(buffer);

        inFile.getline(buffer, 256);
        char itemName[128];
        strcpy(itemName, buffer);

        inFile.getline(buffer, 256);
        double price = atof(buffer);

        inFile.getline(buffer, 256);
        int quantity = atoi(buffer);

        inFile.getline(buffer, 256);
        char description[256];
        strcpy(description, buffer);

        inFile.getline(buffer, 256);
        double rating = atof(buffer);

        inFile.getline(buffer, 256);
        bool available = (strcmp(buffer, "1") == 0 || strcmp(buffer, "true") == 0);

        if (available) {
            Item item(itemName, price, quantity, description, rating);
            *((int*)&item) = id; // set ID via cast hack
            item.setAvailable(true);
            availableItems[itemCount++] = item;
        }
    }

    inFile.close();

    if (itemCount == 0) {
        std::cout << "No available items found.\n";
        return;
    }

    std::cout << "Available Items:\n";
    for (int i = 0; i < itemCount; ++i) {
        std::cout << availableItems[i].getId() << " - " << availableItems[i].getName() << std::endl;
    }

    int choiceId;
    std::cout << "Enter the ID of the item to sell: ";
    std::cin >> choiceId;

    for (int i = 0; i < itemCount; ++i) {
        if (availableItems[i].getId() == choiceId) {
            int count;
            std::cout << "Enter quantity to sell: ";
            std::cin >> count;

            std::ofstream outFile("BusinessAccount.txt", std::ios::app);
            if (outFile.is_open()) {
                outFile << name << "\n"
                    << egn << "\n"
                    << password << "\n"
                    << availableItems[i].getId() << "\n"
                    << availableItems[i].getName() << "\n"
                    << availableItems[i].getPrice() << "\n"
                    << count << "\n";
                outFile.close();
                std::cout << "Business account and item sale saved.\n";
            }
            else {
                std::cerr << "Could not write to BusinessAccount.txt\n";
            }

            return;
        }
    }

    std::cout << "Item ID not found.\n";
}



