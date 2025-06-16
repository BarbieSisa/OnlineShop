#include "BusinessAccount.h"
#include <iostream>
#include<fstream>
#include<string>
using std::cout;
using std::endl;

BusinessAccount::BusinessAccount(const std::string& name, const std::string& egn, const std::string& password)
    : User(name.c_str(), egn.c_str(), password.c_str()) {
}
void BusinessAccount::setSoldItem(const Item& item, int count) {
    soldItem.first = count;
    soldItem.second = item;
}
std::string BusinessAccount::loginBusinessByUsernameAndPassword(const std::string& username, const std::string& password) {
    std::ifstream inFile("BusinessAccount.txt");
    if (!inFile) {
        std::cerr << "❌ Cannot open BusinessAccount.txt\n";
        return "invalid";
    }

    std::string name, egn, storedPassword;
    std::string itemName;
    int itemId, availability;
    double price;

    while (std::getline(inFile, name) &&
        std::getline(inFile, egn) &&
        std::getline(inFile, storedPassword) &&
        (inFile >> itemId) &&
        inFile.ignore() &&                             // след itemId
        std::getline(inFile, itemName) &&
        (inFile >> price) &&
        inFile.ignore() &&                             // след price
        (inFile >> availability)) {
        inFile.ignore();                                  // след availability

        // ДЕБЪГ – махни това после
        std::cout << "[DEBUG] read name: " << name << ", egn: " << egn << ", pass: " << storedPassword << std::endl;

        if (username == name && password == storedPassword) {
            return egn;  // успех
        }
    }

    return "invalid";  // неуспех
}
const std::pair<int, Item>& BusinessAccount::getSoldItem() const {
    return soldItem;
}
void BusinessAccount::viewSoldItem() const {
    cout << "Sold Item Info:" << endl;
    cout << "Quantity: " << soldItem.first << endl;
    cout << "Item Name: " << soldItem.second.getName() << endl;
}
void BusinessAccount::viewProfile() const {
    cout << "Business Profile:\n";
    cout << "Name: " << getName() << endl;
    cout << "EGN: " << getEGN() << endl;
    viewSoldItem();
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
    std::string name, egn, password;
    std::cout << "Enter business name: ";
    std::getline(std::cin >> std::ws, name);
    std::cout << "Enter EGN: ";
    std::getline(std::cin, egn);
    std::cout << "Enter password: ";
    std::getline(std::cin, password);

    std::ifstream inFile("Items.txt");
    if (!inFile) {
        std::cerr << "❌ Cannot open Items.txt\n";
        return;
    }

    std::vector<Item> availableItems;
    std::string line;
    while (getline(inFile, line)) {
        int id = std::stoi(line);
        std::string itemName;
        getline(inFile, itemName);

        getline(inFile, line);
        double price = std::stod(line);

        getline(inFile, line);
        int quantity = std::stoi(line);

        std::string description;
        getline(inFile, description);

        getline(inFile, line);
        double rating = std::stod(line);

        getline(inFile, line);
        bool available = (line == "1" || line == "true");

        if (available) {
            Item item(itemName, price, quantity, description, rating);
            *((int*)&item) = id;
            item.setAvailable(true);
            availableItems.push_back(item);
        }
    }
    inFile.close();

    if (availableItems.empty()) {
        cout << "No available items found.\n";
        return;
    }

    cout << "Available Items:\n";
    for (const auto& item : availableItems) {
        cout << item.getId() << " - " << item.getName() << endl;
    }

    int choiceId;
    cout << "Enter the ID of the item to sell: ";
    std::cin >> choiceId;

    for (const auto& item : availableItems) {
        if (item.getId() == choiceId) {
            int count;
            cout << "Enter quantity to sell: ";
            std::cin >> count;

            std::ofstream outFile("BusinessAccount.txt", std::ios::app);
            if (outFile.is_open()) {
                outFile << name << "\n"
                    << egn << "\n"
                    << password << "\n"
                    << item.getId() << "\n"
                    << item.getName() << "\n"
                    << item.getPrice() << "\n"
                    << count << "\n";
                outFile.close();
                cout << "Business account and item sale saved.\n";
            }
            else {
                std::cerr << "Could not write to BusinessAccount.txt\n";
            }
            return;
        }
    }
    cout << "Item ID not found.\n";
}


