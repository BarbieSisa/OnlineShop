#include "Order.h"
#include "Client.h"
#include <iostream>
#include <fstream>
int Order::nextId = 1;
MyVector<Order> Order::orders;
double discountValue = 0.0;      
bool discountApplied = false;
MyString statusToString(OrderStatus status) {
    switch (status) {
    case OrderStatus::Pending: return MyString("Pending");
    case OrderStatus::Confirmed: return MyString("Confirmed");
    case OrderStatus::Shipped: return MyString("Shipped");
    case OrderStatus::Delivered: return MyString("Delivered");
    default: return MyString("Unknown");
    }
}
Order::Order() : id(nextId++), status(OrderStatus::Pending) {}
int Order::findIndexById(int id) {
    for (size_t i = 0; i < orders.size(); ++i) {
        if (orders[i].id == id) return static_cast<int>(i);
    }
    return -1;
} 
void Order::addItem(const Item& item, int quantity) {
    items.push_back(item);
    quantities.push_back(quantity);
}
int toInt(const char* str) {
    int result = 0;
    int sign = 1;
    int i = 0;

    if (str[0] == '-') {
        sign = -1;
        i++;
    }

    for (; str[i] != '\0'; ++i) {
        if (str[i] < '0' || str[i] > '9') break; // спира при нецифра
        result = result * 10 + (str[i] - '0');
    }

    return result * sign;
}
double toDouble(const char* str) {
    double result = 0.0;
    double frac = 0.0;
    int sign = 1;
    int i = 0;
    bool decimal = false;
    double divisor = 10.0;

    if (str[0] == '-') {
        sign = -1;
        i++;
    }

    for (; str[i] != '\0'; ++i) {
        if (str[i] == '.') {
            decimal = true;
            continue;
        }

        if (str[i] < '0' || str[i] > '9') break;

        if (!decimal) {
            result = result * 10.0 + (str[i] - '0');
        }
        else {
            frac += (str[i] - '0') / divisor;
            divisor *= 10.0;
        }
    }

    return sign * (result + frac);
}
void Order::addItemToOrder(MyVector<Item>& items, MyVector<int>& quantities) {
    char buffer[256];
    int id = -1;

    while (true) {
        std::cout << "Item ID (0 to finish): ";
        std::cin.getline(buffer, sizeof(buffer));
        id = toInt(buffer);

        if (id == 0) break;

        Item item;
        try {
            item = Item::read(id);
        }
        catch (...) {
            std::cout << "Invalid ID. Try again.\n";
            continue;
        }

        std::cout << "Found: " << item.getName().c_str() << "\n";

        std::cout << "How many of '" << item.getName().c_str() << "'? ";
        std::cin.getline(buffer, sizeof(buffer));
        int quantityItem = toInt(buffer);

        if (quantityItem <= 0) {
            std::cout << "Invalid quantity. Try again.\n";
            continue;
        }

        if (quantityItem > item.getQuantity()) {
            std::cout << "Only " << item.getQuantity() << " items available.\n";
            continue;
        }

        items.push_back(item);
        quantities.push_back(quantityItem);

        std::cout << "Added: " << quantityItem << " x '" << item.getName().c_str() << "'\n";
    }

    for (size_t i = 0; i < items.size(); ++i) {
        Item item = items[i];
        int qty = quantities[i];

        int newQuantity = item.getQuantity() - qty;
        item.setQuantity(newQuantity);
        item.setAvailable(newQuantity > 0);
        Item::updateQuantity(item.getId(), qty);
    }

    std::cout.setf(std::ios::fixed);
    std::cout.precision(2);

    double total = 0;

    std::cout << "\nSummary of your order:\n";
    for (size_t i = 0; i < items.size(); ++i) {
        const Item& item = items[i];
        int qty = quantities[i];
        double subtotal = item.getPrice() * qty;

        std::cout << "- " << item.getName().c_str()
            << " x" << qty
            << " = " << subtotal << " BGN\n";
        total += subtotal;
    }

    std::cout << "Total: " << total << " BGN\n";
    std::cout << "Reward Points: " << (total * 0.05) << "\n";
    std::cout << "Finished. Unique items: " << items.size() << "\n";
}
void Order::removeItemFromOrder(MyVector<Item>& items, MyVector<int>& quantities) {
    char buffer[256];
    int id;

    std::cout << "Enter ID of item to remove from order: ";
    std::cin.getline(buffer, sizeof(buffer));
    id = toInt(buffer);

    int foundIndex = -1;
    int totalQuantity = 0;

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i].getId() == id) {
            foundIndex = static_cast<int>(i);
            totalQuantity = quantities[i];
            break;
        }
    }

    if (foundIndex == -1) {
        std::cout << "Item not found in order.\n";
        return;
    }

    std::cout << "There are " << totalQuantity << " of this item in the order.\n";
    std::cout << "How many do you want to remove? ";
    std::cin.getline(buffer, sizeof(buffer));
    int toRemove = toInt(buffer);

    if (toRemove >= totalQuantity) {
        std::cout << "Removing all " << totalQuantity << " of this item.\n";
        items.erase(foundIndex);
        quantities.erase(foundIndex);
    }
    else {
        quantities[foundIndex] -= toRemove;
        std::cout << "Removed " << toRemove << " of item ID " << id << ".\n";
    }
    double total = 0;
    std::cout << "\nUpdated order summary:\n";
    for (size_t i = 0; i < items.size(); ++i) {
        const Item& item = items[i];
        int qty = quantities[i];
        double subtotal = item.getPrice() * qty;
        std::cout << "- " << item.getName().c_str()
            << " x" << qty
            << " = " << subtotal << " BGN\n";
        total += subtotal;
    }

    std::cout << "Total: " << total << " BGN\n";
    std::cout << "Reward Points: " << (total * 0.05) << "\n";
    std::cout << "Remaining items: " << items.size() << "\n";
}
void Order::applyDiscount(const MyString& egn, const MyVector<Item>& items, bool& discountApplied) {
    Client::loadAllClientsFromFile();
    int clientIndex = Client::findIndexByEGN(egn);
    if (clientIndex == -1) {
        std::cout << "Client not found.\n";
        discountApplied = false;
        return;
    }

    Client client = Client::getClientByIndex(clientIndex);
    double total = 0.0;

    for (size_t i = 0; i < items.size(); ++i) {
        total += items[i].getPrice();
    }

    int availablePoints = client.getPoints();
    double maxDiscount = total * 0.5;
    double discount = (availablePoints * 0.01 < maxDiscount) ? (availablePoints * 0.01) : maxDiscount;

    if (discount <= 0.0) {
        std::cout << "Not enough points to apply discount.\n";
        discountApplied = false;
        return;
    }

    Client::updateBalance(egn, discount);
    Client::updatePoints(egn, -static_cast<int>(discount / 0.05));
    std::cout << "Discount of " << discount << " BGN applied.\n";
    discountApplied = true;
}
void Order::removeDiscount(const MyString egn, bool& discountApplied) {
    Client::loadAllClientsFromFile();

    if (!discountApplied) {
        std::cout << "No discount to remove.\n";
        return;
    }

    int refundPoints = static_cast<int>(discountApplied / 0.05);
    Client::updatePoints(egn, refundPoints);
    std::cout << "Removed discount and refunded " << refundPoints << " points.\n";

    discountApplied = false;
}
void Order::viewCart(const MyVector<Item>& items, double discountApplied) {
    Client::loadAllClientsFromFile();
    double total = 0;

    std::cout << "\nCart Contents:\n";
    for (size_t i = 0; i < items.size(); ++i) {
        const Item& item = items[i];
        std::cout << "- " << item.getName().c_str()
            << " - " << item.getPrice() << " BGN\n";
        total += item.getPrice();
    }

    std::cout << "Total without discount: " << total << " BGN\n";
    std::cout << "Discount applied: " << discountApplied << " BGN\n";
    std::cout << "Final total: " << (total - discountApplied) << " BGN\n";
}
void Order::checkoutOrder(const MyVector<Item>& items, const MyString& clientEGN) {
    if (items.size() == 0) {
        std::cout << "No items in order. Order not created.\n";
        return;
    }

    if (orders.size() == 0) {
        loadFromFile();
    }

    Order order;
    order.setClientEGN(clientEGN);
    MyVector<Item> uniqueItems;
    MyVector<int> quantities;

    for (size_t i = 0; i < items.size(); ++i) {
        bool found = false;
        for (size_t j = 0; j < uniqueItems.size(); ++j) {
            if (items[i].getId() == uniqueItems[j].getId()) {
                ++quantities[j];
                found = true;
                break;
            }
        }
        if (!found) {
            uniqueItems.push_back(items[i]);
            quantities.push_back(1);
        }

        order.addItem(items[i]);
    }

    orders.push_back(order);
    std::cout << "New Order Created! ID: " << order.getId() << "\n";

    std::ofstream outFile("Orders.txt", std::ios::app);
    if (!outFile) {
        std::cerr << "Error: Could not open Orders.txt for writing.\n";
        return;
    }

    outFile << order.getId() << "\n";
    outFile << clientEGN.c_str() << "\n";
    outFile << statusToString(order.getStatus()) << "\n";
    outFile << uniqueItems.size() << "\n";

    for (size_t i = 0; i < uniqueItems.size(); ++i) {
        const Item& item = uniqueItems[i];
        int qty = quantities[i];
        Item::updateQuantity(item.getId(), qty);

        outFile << item.getName().c_str() << "\n";
        outFile << item.getPrice() << "\n";
        outFile << qty << "\n";
        outFile << item.getAvailable() << "\n";
    }

    Client::updateBalanceForOrder(clientEGN.c_str(), order.getTotalPrice());
    Client::updatePoints(clientEGN.c_str(), order.getRewardPoints());

    outFile << order.getTotalPrice() << "\n";
    outFile << order.getRewardPoints() << "\n";
    outFile.close();

    std::cout << "\nOrder Summary:\n";
    for (size_t i = 0; i < uniqueItems.size(); ++i) {
        const Item& item = uniqueItems[i];
        int qty = quantities[i];
        std::cout << "- " << item.getName().c_str()
            << " x" << qty
            << " = " << (item.getPrice() * qty) << " BGN\n";
    }

    std::cout << "Total: " << order.getTotalPrice() << " BGN\n";
    std::cout << "Reward Points: " << order.getRewardPoints() << "\n";
}
void Order::removeOrderById(int idToRemove) {
    std::ifstream inFile("Orders.txt");
    if (!inFile) {
        std::cerr << "Cannot open Orders.txt for reading.\n";
        return;
    }

    std::ofstream outFile("Orders_temp.txt");
    if (!outFile) {
        std::cerr << "Cannot open temporary file for writing.\n";
        return;
    }

    char buffer[256];
    int orderId, itemCount;
    double itemPrice, totalPrice, rewardPoints;
    bool found = false;

    while (inFile >> orderId) {
        inFile.ignore();

        inFile.getline(buffer, sizeof(buffer));
        MyString clientEGN = buffer;

        inFile.getline(buffer, sizeof(buffer));
        MyString status = buffer;

        inFile >> itemCount;
        inFile.ignore();

        MyVector<MyString> itemNames;
        MyVector<double> itemPrices;

        for (int i = 0; i < itemCount; ++i) {
            inFile.getline(buffer, sizeof(buffer));
            itemNames.push_back(MyString(buffer));
            inFile >> itemPrice;
            itemPrices.push_back(itemPrice);
            inFile.ignore();
        }

        inFile >> totalPrice >> rewardPoints;
        inFile.ignore();

        if (orderId == idToRemove) {
            found = true;
            continue;
        }

        outFile << orderId << '\n';
        outFile << clientEGN.c_str() << '\n';
        outFile << status.c_str() << '\n';
        outFile << itemCount << '\n';

        for (int i = 0; i < itemCount; ++i) {
            outFile << itemNames[i].c_str() << '\n';
            outFile << itemPrices[i] << '\n';
        }

        outFile << totalPrice << '\n';
        outFile << rewardPoints << '\n';
    }

    inFile.close();
    outFile.close();

    if (found) {
        std::remove("Orders.txt");
        std::rename("Orders_temp.txt", "Orders.txt");
        std::cout << "Order ID " << idToRemove << " was successfully removed.\n";
    }
    else {
        std::remove("Orders_temp.txt");
        std::cout << "Order ID " << idToRemove << " was not found.\n";
    }
}
void Order::approveRefund(int orderIdToMove) {
    std::ifstream inFile("Refunds.txt");
    if (!inFile) {
        std::cerr << "Cannot open Refunds.txt for reading.\n";
        return;
    }

    std::ofstream outFile("Reject_temp.txt");
    std::ofstream refundFile("RefundUser.txt", std::ios::app);
    if (!outFile || !refundFile) {
        std::cerr << "Failed to open output files.\n";
        return;
    }

    char buffer[256];
    int orderId, itemCount;
    double itemPrice, total, points;
    bool found = false;
    MyString clientEGN, status, reason;
    MyVector<MyString> itemNames;
    MyVector<double> itemPrices;

    while (inFile >> orderId) {
        inFile.ignore();
        inFile.getline(buffer, sizeof(buffer));
        clientEGN = buffer;

        inFile.getline(buffer, sizeof(buffer));
        status = buffer;

        inFile >> itemCount;
        inFile.ignore();

        itemNames.clear();
        itemPrices.clear();

        for (int i = 0; i < itemCount; ++i) {
            inFile.getline(buffer, sizeof(buffer));
            itemNames.push_back(MyString(buffer));
            inFile >> itemPrice;
            itemPrices.push_back(itemPrice);
            inFile.ignore();
        }

        inFile >> total >> points;
        inFile.ignore();

        inFile.getline(buffer, sizeof(buffer));
        reason = buffer;

        if (orderId == orderIdToMove) {
            found = true;
            refundFile << orderId << '\n'
                << clientEGN.c_str() << '\n'
                << status.c_str() << '\n'
                << itemCount << '\n';

            for (int i = 0; i < itemCount; ++i) {
                refundFile << itemNames[i].c_str() << '\n'
                    << itemPrices[i] << '\n';
            }

            refundFile << total << '\n'
                << points << '\n'
                << reason.c_str() << '\n';
        }
        else {
            outFile << orderId << '\n'
                << clientEGN.c_str() << '\n'
                << status.c_str() << '\n'
                << itemCount << '\n';

            for (int i = 0; i < itemCount; ++i) {
                outFile << itemNames[i].c_str() << '\n'
                    << itemPrices[i] << '\n';
            }

            outFile << total << '\n'
                << points << '\n'
                << reason.c_str() << '\n';
        }
    }

    inFile.close();
    outFile.close();
    refundFile.close();

    if (found) {
        std::remove("Refunds.txt");
        std::rename("Reject_temp.txt", "Refunds.txt");
        std::cout << "Order ID " << orderIdToMove << " moved to RefundUser.txt\n";
        Client::updateBalance(clientEGN, total);
    }
    else {
        std::remove("Reject_temp.txt");
        std::cout << "Order ID " << orderIdToMove << " was not found in Refunds.txt\n";
    }
}
void Order::setClientEGN(const MyString& egn) {
    clientEGN = egn;
}
int Order::getId() const {
    return id;
}
OrderStatus Order::getStatus() const {
    return status;
}
const MyVector<Item>& Order::getItems () const {
    return items;
}
const MyVector<int>& Order::getQuantities() const {
    return quantities;
}
double Order::getTotalPrice() const {
    double total = 0.0;
    for (size_t i = 0; i < items.size(); ++i) {
        total += items[i].getPrice() * quantities[i];
    }
    return total;
}
double Order::getRewardPoints() const {
    return static_cast<double>(getTotalPrice()*0.01);
}
void Order::saveToFile() {
    std::ofstream outFile("Orders.txt");
    if (!outFile.is_open()) {
        std::cerr << "Failed to open Orders.txt for writing.\n";
        return;
    }

    for (size_t i = 0; i < orders.size(); ++i) {
        const Order& order = orders[i];
        outFile << order.getId() << "\n";
        outFile << order.clientEGN << "\n";
        outFile << statusToString(order.getStatus()).c_str() << "\n";
        outFile << order.getItems().size() << "\n";

        const MyVector<Item>& items = order.getItems();
        const MyVector<int>& quantities = order.getQuantities();

        for (size_t j = 0; j < items.size(); ++j) {
            const Item& item = items[j];
            int quantity = quantities[j];

            outFile << item.getId() << "\n";
            outFile << item.getName().c_str() << "\n";
            outFile << item.getPrice() << "\n";
            outFile << quantity << "\n";
            outFile << item.getAvailable() << "\n";
        }

        outFile << order.getTotalPrice() << "\n";
        outFile << order.getRewardPoints() << "\n";
    }

    outFile.close();
    std::cout << "Orders saved to file successfully.\n";
}
void Order::updateOrderStatusById(int id) {
    loadFromFile(); 
    bool found = false;
    for (size_t i = 0; i < orders.size(); ++i) {
        if (orders[i].getId() == id) {
            found = true;
            std::cout << "Current status: " << statusToString(orders[i].getStatus()).c_str() << "\n";
            std::cout << "Select new status:\n";
            std::cout << "1. Pending\n";
            std::cout << "2. Confirmed\n";
            std::cout << "3. Shipped\n";
            std::cout << "4. Delivered\n";
            std::cout << "Choice: ";

            int choice;
            std::cin >> choice;

            switch (choice) {
            case 1: orders[i].setStatus(OrderStatus::Pending); break;
            case 2: orders[i].setStatus(OrderStatus::Confirmed); break;
            case 3: orders[i].setStatus(OrderStatus::Shipped); break;
            case 4: orders[i].setStatus(OrderStatus::Delivered); break;
            default:
                std::cerr << "Invalid choice. Status not updated.\n";
                return;
            }
            saveToFile(); 
            std::cout << "Order status updated successfully.\n";
            return;
        }
    }

    if (!found) {
        std::cerr << "Order with ID " << id << " not found.\n";
    }
}
void Order::setStatus(OrderStatus newStatus) {
    status = newStatus;
}
void Order::setId(int newId) {
    id = newId;
}
void Order::listAllOrders() {
    int choice;
    std::cout << "Sort orders by:\n"
        << "1. Rating\n"
        << "2. Price (ascending)\n"
        << "3. Price (descending)\n"
        << "4. Alphabetical order\n"
        << "Choice: ";
    std::cin >> choice;
    loadFromFile(); 
    int n = orders.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            bool shouldSwap = false;

            switch (choice) {
            case 1: {
                double ratingA = 0, ratingB = 0;
                const MyVector<Item>& itemsA = orders[j].getItems();
                const MyVector<Item>& itemsB = orders[j + 1].getItems();
                for (int k = 0; k < itemsA.size(); ++k)
                    ratingA += itemsA[k].getRating();
                for (int k = 0; k < itemsB.size(); ++k)
                    ratingB += itemsB[k].getRating();
                shouldSwap = ratingA < ratingB;
                break;
            }
            case 2:
                shouldSwap = orders[j].getTotalPrice() > orders[j + 1].getTotalPrice();
                break;

            case 3:
                shouldSwap = orders[j].getTotalPrice() < orders[j + 1].getTotalPrice();
                break;

            case 4: {
                const MyVector<Item>& itemsA = orders[j].getItems();
                const MyVector<Item>& itemsB = orders[j + 1].getItems();
                if (itemsA.size() > 0 && itemsB.size() > 0) {
                    MyString nameA = itemsA[0].getName();
                    MyString nameB = itemsB[0].getName();
                    shouldSwap = nameA > nameB;
                }
                break;
            }

            default:
                std::cout << "Invalid choice. Displaying unsorted.\n";
                i = n; // force exit
                j = n;
                break;
            }

            if (shouldSwap) {
                Order temp = orders[j];
                orders[j] = orders[j + 1];
                orders[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < orders.size(); ++i) {
        orders[i].display();
        std::cout << "-----------------------------\n";
    }
}
MyVector<Item>& Order::getItemsRef() {
    return items;
}
MyVector<int>& Order::getQuantitiesRef() {
    return quantities;
}
void Order::loadFromFile() {
    std::ifstream inFile("Orders.txt");
    if (!inFile.is_open()) {
        std::cerr << "Cannot open Orders.txt\n";
        return;
    }

    orders.clear();
    nextId = 1;

    int orderId, itemCount;
    double totalPrice, rewardPoints;

    char buffer[256]; 

    while (inFile >> orderId) {
        inFile.ignore();

        inFile.getline(buffer, 256);
        MyString clientEGN(buffer);

        inFile.getline(buffer, 256);
        MyString statusStr(buffer);

        inFile >> itemCount;
        inFile.ignore();

        Order o;
        o.setId(orderId);
        o.setClientEGN(clientEGN);

        if (statusStr == "Pending") o.setStatus(OrderStatus::Pending);
        else if (statusStr == "Confirmed") o.setStatus(OrderStatus::Confirmed);
        else if (statusStr == "Shipped") o.setStatus(OrderStatus::Shipped);
        else if (statusStr == "Delivered") o.setStatus(OrderStatus::Delivered);

        for (int i = 0; i < itemCount; ++i) {
            inFile.getline(buffer, 256);
            MyString itemName(buffer);

            double itemPrice;
            inFile >> itemPrice;
            inFile.ignore();

            Item item;
            item.setName(itemName);
            item.setPrice(itemPrice);

            o.getItemsRef().push_back(item);
            o.getQuantitiesRef().push_back(1);
        }

        inFile >> totalPrice >> rewardPoints;
        inFile.ignore();

        if (findIndexById(orderId) == -1) {
            orders.push_back(o);
        }

        if (orderId >= nextId) {
            nextId = orderId + 1;
        }
    }

    inFile.close();
}
void Order::loadFromFileRefunds() {
    std::ifstream inFile("Refunds.txt");
    if (!inFile.is_open()) {
        std::cerr << "Cannot open Refunds.txt\n";
        return;
    }

    orders.clear();
    nextId = 1;

    int orderId;
    char buffer[256];
    int itemCount;
    double totalPrice, rewardPoints;

    while (inFile >> orderId) {
        inFile.ignore();

        inFile.getline(buffer, sizeof(buffer));
        MyString clientEGN(buffer);

        inFile.getline(buffer, sizeof(buffer));
        MyString statusStr(buffer);

        inFile >> itemCount;
        inFile.ignore();

        Order o;
        o.setId(orderId);
        o.setClientEGN(clientEGN);

        if (statusStr == "Pending") o.setStatus(OrderStatus::Pending);
        else if (statusStr == "Confirmed") o.setStatus(OrderStatus::Confirmed);
        else if (statusStr == "Shipped") o.setStatus(OrderStatus::Shipped);
        else if (statusStr == "Delivered") o.setStatus(OrderStatus::Delivered);

        for (int i = 0; i < itemCount; ++i) {
            inFile.getline(buffer, sizeof(buffer));
            MyString itemName(buffer);

            double itemPrice;
            inFile >> itemPrice;
            inFile.ignore();

            Item item;
            item.setName(itemName);
            item.setPrice(itemPrice);

            o.getItemsRef().push_back(item);
            o.getQuantitiesRef().push_back(1);  // default quantity = 1
        }

        inFile >> totalPrice >> rewardPoints;
        inFile.ignore();

        if (findIndexById(orderId) == -1) {
            orders.push_back(o);
        }

        if (orderId >= nextId) {
            nextId = orderId + 1;
        }
    }

    inFile.close();
}
void Order::display() const {
    std::cout << "Order ID: " << id << "\nStatus: " << statusToString(status) << "\nItems:\n";

    for (size_t i = 0; i < items.size(); ++i) {
        const Item& item = items[i];
        int quantity = quantities[i];
        std::cout << "- " << item.getName()
            << " x" << quantity
            << " = " << item.getPrice() * quantity << " BGN\n";
    }

    std::cout << "Total: " << getTotalPrice() << " BGN\n";
    std::cout << "Reward Points: " << getRewardPoints() << "\n";
}
void Order::displayRejectedOrders() {
    std::ifstream inFile("Reject.txt");
    if (!inFile.is_open()) {
        std::cerr << "Cannot open Reject.txt\n";
        return;
    }

    std::cout << "\nRejected Orders:\n";

    int id;
    char egnBuffer[64], statusBuffer[64], itemNameBuffer[128], reasonBuffer[256];
    int itemCount;
    double itemPrice, total, points;

    while (inFile >> id) {
        inFile.ignore(); // Skip newline
        inFile.getline(egnBuffer, sizeof(egnBuffer));
        inFile.getline(statusBuffer, sizeof(statusBuffer));
        inFile >> itemCount;
        inFile.ignore();

        MyVector<Item> items;

        for (int i = 0; i < itemCount; ++i) {
            inFile.getline(itemNameBuffer, sizeof(itemNameBuffer));
            inFile >> itemPrice;
            inFile.ignore();

            Item item;
            item.setName(itemNameBuffer);
            item.setPrice(itemPrice);
            items.push_back(item);
        }

        inFile >> total >> points;
        inFile.ignore();
        inFile.getline(reasonBuffer, sizeof(reasonBuffer));

        // Display
        std::cout << "\nOrder ID: " << id << "\n";
        std::cout << "Client EGN: " << egnBuffer << "\n";
        std::cout << "Status: " << statusBuffer << "\n";
        std::cout << "Items:\n";

        for (size_t i = 0; i < items.size(); ++i) {
            const Item& item = items[i];
            std::cout << "- " << item.getName() << " - " << item.getPrice() << " BGN\n";
        }

        std::cout << "Total Price: " << total << " BGN\n";
        std::cout << "Reward Points: " << points << "\n";
        std::cout << "Rejection Reason: " << reasonBuffer << "\n";
        std::cout << "----------------------------\n";
    }

    inFile.close();
}
void Order::displayRefundedOrdersUsers(const MyString& currentEGN) {
    std::ifstream inFile("RefundUser.txt");
    if (!inFile.is_open()) {
        std::cerr << "Cannot open RefundUser.txt\n";
        return;
    }

    std::cout << "\nRefunded Orders for EGN: " << currentEGN.c_str() << "\n";

    int id;
    char egnBuffer[64], statusBuffer[64], itemNameBuffer[128], reasonBuffer[256];
    int itemCount;
    double itemPrice, total, points;
    bool found = false;

    while (inFile >> id) {
        inFile.ignore(); // Skip newline
        inFile.getline(egnBuffer, sizeof(egnBuffer));
        inFile.getline(statusBuffer, sizeof(statusBuffer));
        inFile >> itemCount;
        inFile.ignore();

        MyVector<Item> items;

        for (int i = 0; i < itemCount; ++i) {
            inFile.getline(itemNameBuffer, sizeof(itemNameBuffer));
            inFile >> itemPrice;
            inFile.ignore();

            Item item;
            item.setName(itemNameBuffer);
            item.setPrice(itemPrice);
            items.push_back(item);
        }

        inFile >> total >> points;
        inFile.ignore();
        inFile.getline(reasonBuffer, sizeof(reasonBuffer));

        MyString egnStr(egnBuffer);
        if (egnStr != currentEGN) continue;

        found = true;
        std::cout << "\nOrder ID: " << id << "\n";
        std::cout << "Client EGN: " << egnStr.c_str() << "\n";
        std::cout << "Status: " << statusBuffer << "\n";
        std::cout << "Items:\n";
        for (size_t i = 0; i < items.size(); ++i) {
            const Item& item = items[i];
            std::cout << "- " << item.getName() << " - " << item.getPrice() << " BGN\n";
        }
        std::cout << "Total Price: " << total << " BGN\n";
        std::cout << "Reward Points: " << points << "\n";
        std::cout << "Refund Reason: " << reasonBuffer << "\n";
        std::cout << "----------------------------\n";
    }

    if (!found) {
        std::cout << "No refunded orders found for this EGN.\n";
    }

    inFile.close();
}
void Order::displayRefundedOrders() {
    std::ifstream inFile("Refunds.txt");
    if (!inFile.is_open()) {
        std::cerr << "Cannot open Refunds.txt\n";
        return;
    }

    std::cout << "\nRefunded Orders:\n";

    int id, itemCount;
    double itemPrice, total, points;

    char egnBuffer[64];
    char statusBuffer[64];
    char itemNameBuffer[128];
    char reasonBuffer[256];

    while (inFile >> id) {
        inFile.ignore();
        inFile.getline(egnBuffer, sizeof(egnBuffer));
        inFile.getline(statusBuffer, sizeof(statusBuffer));
        inFile >> itemCount;
        inFile.ignore();

        MyVector<Item> items;

        for (int i = 0; i < itemCount; ++i) {
            inFile.getline(itemNameBuffer, sizeof(itemNameBuffer));
            inFile >> itemPrice;
            inFile.ignore();

            Item item;
            item.setName(itemNameBuffer);
            item.setPrice(itemPrice);
            items.push_back(item);
        }

        inFile >> total >> points;
        inFile.ignore();
        inFile.getline(reasonBuffer, sizeof(reasonBuffer));
        std::cout << "\nOrder ID: " << id << "\n";
        std::cout << "Client EGN: " << egnBuffer << "\n";
        std::cout << "Status: " << statusBuffer << "\n";
        std::cout << "Items:\n";

        for (size_t i = 0; i < items.size(); ++i) {
            std::cout << "- " << items[i].getName()
                << " - " << items[i].getPrice() << " BGN\n";
        }

        std::cout << "Total Price: " << total << " BGN\n";
        std::cout << "Reward Points: " << points << "\n";
        std::cout << "Refund Reason: " << reasonBuffer << "\n";
        std::cout << "----------------------------\n";
    }

    inFile.close();
}
void Order::rejectRefundOrderById(int id, const MyString& reason) {
    loadFromFileRefunds();  // Зареждане на поръчките от Refunds.txt

    int index = findIndexById(id);
    if (index == -1) {
        std::cerr << "No order found with ID: " << id << "\n";
        return;
    }

    const Order& order = orders[index];

    std::ofstream outFile("RejectRefund.txt", std::ios::app);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open RejectRefund.txt for writing.\n";
        return;
    }

    outFile << order.getId() << "\n";
    outFile << order.getClientEGN().c_str() << "\n";
    outFile << statusToString(order.getStatus()).c_str() << "\n";
    outFile << order.getItems().size() << "\n";

    const MyVector<Item>& items = order.getItems();
    const MyVector<int>& quantities = order.getQuantities();

    for (size_t i = 0; i < items.size(); ++i) {
        outFile << items[i].getName().c_str() << "\n";
        outFile << items[i].getPrice() << "\n";
    }

    outFile << order.getTotalPrice() << "\n";
    outFile << order.getRewardPoints() << "\n";
    outFile << reason.c_str() << "\n";
    outFile.close();

    std::cout << "Order ID " << id << " successfully written to RejectRefund.txt\n";
}
void Order::rejectOrderById(int id, const MyString& reason) {
    loadFromFile();  
    bool found = false;
    int index = -1;

    for (int i = 0; i < orders.size(); ++i) {
        if (orders[i].getId() == id) {
            found = true;
            index = i;
            break;
        }
    }

    if (!found) {
        std::cout << "No order found with ID: " << id << "\n";
        return;
    }

    Order& order = orders[index];

    std::fstream outFile("RejectRefund.txt", std::ios::out | std::ios::app);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open RejectRefund.txt\n";
        return;
    }

    outFile << order.getId() << "\n";
    outFile << order.getClientEGN().c_str() << "\n";
    outFile << statusToString(order.getStatus()).c_str() << "\n";

    const MyVector<Item>& items = order.getItems();
    const MyVector<int>& quantities = order.getQuantities();

    outFile << items.size() << "\n";

    for (int i = 0; i < items.size(); ++i) {
        outFile << items[i].getName().c_str() << "\n";
        outFile << items[i].getPrice() << "\n";
        outFile << quantities[i] << "\n";
    }

    outFile << order.getTotalPrice() << "\n";
    outFile << order.getRewardPoints() << "\n";
    outFile << reason.c_str() << "\n";

    outFile.close();
    std::cout << "Order ID " << id << " successfully written to RejectRefund.txt\n";
}
void Order::refundOrderById(int id) {
    loadFromFile();  
    bool found = false;
    int index = -1;

    for (int i = 0; i < orders.size(); ++i) {
        if (orders[i].getId() == id) {
            found = true;
            index = i;
            break;
        }
    }

    if (!found) {
        std::cout << "No order found with ID: " << id << "\n";
        return;
    }

    Order& order = orders[index];

    std::fstream outFile("Refunds.txt", std::ios::out | std::ios::app);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open Refunds.txt\n";
        return;
    }

    outFile << order.getId() << "\n";
    outFile << order.getClientEGN().c_str() << "\n";
    outFile << statusToString(order.getStatus()).c_str() << "\n";
    outFile << order.getItems().size() << "\n";

    const MyVector<Item>& items = order.getItems();
    for (int i = 0; i < items.size(); ++i) {
        outFile << items[i].getName().c_str() << "\n";
        outFile << items[i].getPrice() << "\n";
    }

    outFile << order.getTotalPrice() << "\n";
    outFile << order.getRewardPoints() << "\n";

    outFile.close();

    std::cout << "Order ID " << id << " successfully written to Refunds.txt\n";
}
void Order::displayPending() {
    loadFromFile();  
    bool found = false;
    for (const auto& order : orders) {
        if (order.getStatus() == OrderStatus::Pending) {
            std::cout << "-----------------------------\n";
            order.display();
            found = true;
        }
    }
}
void Order::displayShipped() {
    loadFromFile();  
    bool found = false;
    for (const auto& order : orders) {
        if (order.getStatus() == OrderStatus::Shipped) {
            std::cout << "-----------------------------\n";
            order.display();
            found = true;
        }
    }
}
void Order::displayDelivered() {
    loadFromFile();  
    bool found = false;
    for (const auto& order : orders) {
        if (order.getStatus() == OrderStatus::Delivered) {
            std::cout << "-----------------------------\n";
            order.display();
            found = true;
        }
    }
}
void Order::displayConfirmed() {
    loadFromFile();  
    bool found = false;
    for (const auto& order : orders) {
        if (order.getStatus() == OrderStatus::Confirmed) {
            std::cout << "-----------------------------\n";
            order.display();
            found = true;
        }
    }
}
void Order::UpdatePointsWhenConfirmed(int orderId) {
    loadFromFile(); // зарежда orders от файл

    for (int i = 0; i < orders.size(); ++i) {
        Order& order = orders[i];

        if (order.getId() == orderId) {
            if (order.getStatus() == OrderStatus::Confirmed) {
                MyString egn = order.clientEGN;
                double pointsToAdd = order.getTotalPrice() * 0.5;

                Client::updatePoints(egn, pointsToAdd); 

                order.setStatus(OrderStatus::Shipped);
                saveToFile(); 

                std::cout << "Order confirmed and points updated!\n";
                return;
            }
            else {
                std::cout << "Order is not in 'Confirmed' state.\n";
                return;
            }
        }
    }

    std::cout << "Order with ID " << orderId << " not found.\n";
}
void Order::displayUser(const MyString& egn) {
    loadFromFile();  

    bool found = false;

    for (int i = 0; i < orders.size(); ++i) {
        const Order& order = orders[i];

        if (strcmp(order.clientEGN.c_str(), egn.c_str()) == 0 && order.getStatus() == OrderStatus::Confirmed) {
            std::cout << "-----------------------------\n";
            order.display();
            found = true;
        }
    }

    if (!found) {
        std::cout << "No confirmed orders found for this EGN.\n";
    }
}
void Order::displayUserSum(const MyString& egn) {
    loadFromFile();

    bool found = false;
    double totalSum = 0.0;

    for (int i = 0; i < orders.size(); ++i) {
        const Order& order = orders[i];

        if (strcmp(order.clientEGN.c_str(), egn.c_str()) == 0) {
            std::cout << "-----------------------------\n";
            order.display();
            totalSum += order.getTotalPrice();
            found = true;
        }
    }

    std::cout << "The total sum of the orders of the current user is " << totalSum << "\n";

    if (!found) {
        std::cout << "No confirmed orders found for this EGN.\n";
    }
}
void Order::displayUserDelivered(const MyString& egn) {
    loadFromFile();  

    bool found = false;
    for (int i = 0; i < orders.size(); ++i) {
        const Order& order = orders[i];

        if (strcmp(order.clientEGN.c_str(), egn.c_str()) == 0 &&
            order.getStatus() == OrderStatus::Delivered) {
            std::cout << "-----------------------------\n";
            order.display();
            found = true;
        }
    }

    if (!found) {
        std::cout << "No delivered orders found for this EGN.\n";
    }
}
void Order::displayAllOrdersFromFile() {
    loadFromFile(); // Зарежда orders в паметта

    if (orders.empty()) {
        std::cout << "❌ No orders found.\n";
        return;
    }

    for (const auto& order : orders) {
        std::cout << "-----------------------------\n";
        order.display();
    }
    std::cout << "-----------------------------\n";
}
