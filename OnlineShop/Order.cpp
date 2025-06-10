#include "Order.h"
#include "Client.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <algorithm>

int Order::nextId = 1;
std::vector<Order> Order::orders;

std::string statusToString(OrderStatus status) {
    switch (status) {
    case OrderStatus::Pending: return "Pending";
    case OrderStatus::Shipped: return "Shipped";
    case OrderStatus::Delivered: return "Delivered";
    default: return "Unknown";
    }
}

Order::Order() : id(nextId++), status(OrderStatus::Pending) {}
int Order::findIndexById(int id) {
    for (size_t i = 0; i < orders.size(); ++i) {
        if (orders[i].id == id) return static_cast<int>(i);
    }
    return -1;
}

void Order::addItemToOrder(std::vector<Item>& items) {
    int id;
    std::map<int, int> quantityMap;  // itemId -> quantity
    std::map<int, Item> itemMap;

    std::cout << "Enter item IDs to add to order (0 to finish):\n";
    while (true) {
        std::cout << "Item ID: ";
        std::cin >> id;
        if (id == 0) break;

        try {
            std::cout << "Reading item with ID " << id << "...\n";
            Item item = Item::read(id);

            std::cout << "Item found: " << item.getName() << "\n";

            int quantityItem;
            std::cout << "How many of '" << item.getName() << "' do you want to add? ";
            std::cin >> quantityItem;

            if (quantityItem > item.getQuantity()) {
                std::cout << "Only " << item.getQuantity() << " items available. Cannot proceed.\n";
                continue;
            }

            for (int i = 0; i < quantityItem; ++i) {
                items.emplace_back(item);
            }

            quantityMap[id] += quantityItem;
            itemMap[id] = item;

            item.setQuantity(item.getQuantity() - quantityItem);
            item.setAvailable(item.getQuantity() > 0);
            item.updateQuantity(id, quantityItem);

            std::cout << "✅ " << quantityItem << " x '" << item.getName() << "' added.\n";
        }
        catch (const std::exception& e) {
            std::cerr << "❌ Error: " << e.what() << "\n";
        }
    }

    // 📦 Summary
    std::cout << "\n📦 Summary of your order:\n";
    double total = 0;
    for (const auto& pair : quantityMap) {
        int itemId = pair.first;
        int qty = pair.second;
        const Item& item = itemMap[itemId];
        double subtotal = item.getPrice() * qty;
        std::cout << "- " << item.getName()
            << " x" << qty
            << " = " << subtotal << " BGN\n";
        total += subtotal;
    }

    std::cout << "🧾 Total: " << total << " BGN\n";
    std::cout << "🎁 Reward Points: " << (total * 0.05) << "\n";
    std::cout << "Finished adding items. Total: " << items.size() << " item(s).\n";
}

void Order::removeItemFromOrder(std::vector<Item>& items) {
    int id;
    std::cout << "Enter ID of item to remove from order: ";
    std::cin >> id;

    // Count how many of this item exist in the order
    int count = std::count_if(items.begin(), items.end(), [id](const Item& item) {
        return item.getId() == id;
        });

    if (count == 0) {
        std::cout << "Item not found in order.\n";
        return;
    }

    std::cout << "There are " << count << " of this item in the order.\n";
    std::cout << "How many do you want to remove? ";
    int toRemove;
    std::cin >> toRemove;

    if (toRemove > count) {
        std::cout << "You only have " << count << " of this item. Removing all.\n";
        toRemove = count;
    }

    // Remove only the requested quantity
    int removed = 0;
    auto it = items.begin();
    while (it != items.end() && removed < toRemove) {
        if (it->getId() == id) {
            it = items.erase(it);
            removed++;
        }
        else {
            ++it;
        }
    }

    std::cout << "✅ Removed " << removed << " item(s) with ID " << id << " from the order.\n";

    // 🔁 Show updated summary
    std::map<int, int> quantityMap;
    std::map<int, Item> itemMap;
    for (const auto& item : items) {
        quantityMap[item.getId()]++;
        itemMap[item.getId()] = item;
    }

    std::cout << "\n📦 Updated order summary:\n";
    double total = 0;
    for (const auto& pair : quantityMap) {
        const Item& item = itemMap[pair.first];
        int qty = pair.second;
        double subtotal = item.getPrice() * qty;
        std::cout << "- " << item.getName()
            << " x" << qty
            << " = " << subtotal << " BGN\n";
        total += subtotal;
    }

    std::cout << "🧾 Total: " << total << " BGN\n";
    std::cout << "🎁 Reward Points: " << (total * 0.05) << "\n";
    std::cout << "Finished removing. Remaining: " << items.size() << " item(s).\n";
}

void Order::applyDiscount(const std::string& egn, const std::vector<Item>& items, bool& discountApplied) {

    Client::loadAllClientsFromFile();
    int clientIndex = Client::findIndexByEGN(egn);
    if (clientIndex == -1) {
        std::cerr << "❌ Client not found.\n";
        discountApplied = false;
        return;
    }

    Client client = Client::getClientByIndex(clientIndex);
    double total = 0.0;

    for (const auto& item : items) {
        total += item.getPrice();
    }

    int availablePoints = client.getPoints();
    double maxDiscount = total * 0.5;
    double discount = std::min(availablePoints * 0.01, maxDiscount);  // 1 точка = 0.01 лв

    if (discount <= 0.0) {
        std::cout << "❗ Not enough points to apply discount.\n";
        discountApplied = false;
        return;
    }
    //  Client::loadAllClientsFromFile();
    Client::updateBalance(egn, discount);        // Връщаме сумата като баланс
    Client::updatePoints(egn, -static_cast<int>(discount / 0.05)); // Изваждаме използваните точки
    std::cout << "✅ Discount of " << discount << " BGN applied.\n";
    discountApplied = true;
}
void removeDiscount(std::string egn, bool& discountApplied) {
    Client::loadAllClientsFromFile();
    if (discountApplied == 0) {
        std::cout << "⚠️ No discount to remove.\n";
        return;
    }

    int refundPoints = static_cast<int>(discountApplied / 0.05);
    Client::updatePoints(egn, refundPoints);
    std::cout << "♻️ Removed discount and refunded " << refundPoints << " points.\n";

    discountApplied = 0; // Reset applied discount
}
 void Order::viewCart(const std::vector<Item>& items, bool discountApplied) {
    Client::loadAllClientsFromFile();
    double total = 0;
    std::cout << "\n🛒 Cart Contents:\n";
    for (const auto& item : items) {
        std::cout << "- " << item.getName() << " - " << item.getPrice() << " BGN\n";
        total += item.getPrice();
    }

    std::cout << "📦 Total without discount: " << total << " BGN\n";
    std::cout << "🎯 Discount applied: " << discountApplied << " BGN\n";
    std::cout << "💳 Final total: " << (total - discountApplied) << " BGN\n";
}
void Order::checkoutOrder(const std::vector<Item>& items, const std::string& clientEGN) {
    if (items.empty()) {
        std::cout << "No items in order. Order not created.\n";
        return;
    }

    if (orders.empty()) {
        loadFromFile(); // Ensure orders and nextId are correct
    }

    Order order;
    order.setClientEGN(clientEGN);

    std::map<int, int> itemQuantities;
    std::map<int, Item> itemDetails;

    for (const auto& item : items) {
        order.addItem(item);
        itemQuantities[item.getId()]++;
        itemDetails[item.getId()] = item;
    }

    // Save order in memory
    orders.push_back(order);
    std::cout << "✅ New Order Created! ID: " << order.getId() << "\n";

    // Save order to file
    std::ofstream outFile("Orders.txt", std::ios::app);
    if (outFile.is_open()) {
        outFile << order.getId() << "\n";
        outFile << clientEGN << "\n";
        outFile << statusToString(order.getStatus()) << "\n";
        outFile << itemQuantities.size() << "\n";  // Unique item count

        for (const auto& pair : itemQuantities) {
            int itemId = pair.first;
            int qty = pair.second;
            Item& item = itemDetails[itemId];

            // Update quantity in file
            Item::updateQuantity(itemId, qty);

            outFile << item.getName() << "\n"
                << item.getPrice() << "\n"
                << item.getAvailable() << "\n";  // Save availability
        }
        Client::updateBalanceForOrder(clientEGN, order.getTotalPrice());
        Client::updatePoints(clientEGN, order.getRewardPoints());
        outFile << order.getTotalPrice() << "\n";
        outFile << order.getRewardPoints() << "\n";
        outFile.close();
    }
    else {
        std::cerr << "❌ Error: Could not open Orders.txt for writing.\n";
        return;
    }

    // 🎉 Display summary to user
    std::cout << "\n🧾 Order Summary:\n";
    for (const auto& pair : itemQuantities) {
        const Item& item = itemDetails[pair.first];
        int qty = pair.second;
        std::cout << "- " << item.getName()
            << " x" << qty
            << " = " << (item.getPrice() * qty) << " BGN\n";
    }

    std::cout << "📦 Total: " << order.getTotalPrice() << " BGN\n";
    std::cout << "🎁 Reward Points: " << order.getRewardPoints() << "\n";
}



void Order::addItem(const Item& item) {
    items.emplace_back(item, 1);
}
void Order::setClientEGN(const std::string& egn) {
    clientEGN = egn;
}
int Order::getId() const {
    return id;
}
OrderStatus Order::getStatus() const {
    return status;
}
const std::vector<std::pair<Item, int>>& Order::getItems() const {
    return items;
}
double Order::getTotalPrice() const {
    double total = 0;
    for (const auto& pair : items) {
        total += pair.first.getPrice() * pair.second;
    }
    return total;
}
double Order::getRewardPoints() const {
    return static_cast<double>(getTotalPrice()*0.01);
}
void Order::saveToFile() {
    std::ofstream outFile("Orders.txt");
    if (!outFile.is_open()) {
        std::cerr << "❌ Failed to open Orders.txt for writing.\n";
        return;
    }

    for (const auto& order : orders) {
        outFile << order.getId() << "\n";
        outFile << order.clientEGN << "\n";
        outFile << statusToString(order.getStatus()) << "\n";
        outFile << order.getItems().size() << "\n";

        for (const auto& pair : order.getItems()) {
            const Item& item = pair.first;
            outFile << item.getName() << "\n";
            outFile << item.getPrice() << "\n";
        }

        outFile << order.getTotalPrice() << "\n";
        outFile << order.getRewardPoints() << "\n";
    }

    outFile.close();
    std::cout << "✅ Orders saved to file successfully.\n";
}
void Order::updateOrderStatusById(int id) {
    loadFromFile(); // Ensure all orders are loaded

    auto it = std::find_if(orders.begin(), orders.end(), [id](const Order& o) {
        return o.getId() == id;
        });

    if (it == orders.end()) {
        std::cerr << "❌ Order with ID " << id << " not found.\n";
        return;
    }

    std::cout << "Current status: " << statusToString(it->getStatus()) << "\n";
    std::cout << "Select new status:\n"
        << "1. Pending\n"
        << "2. Shipped\n"
        << "3. Delivered\n"
        << "Choice: ";

    int choice;
    std::cin >> choice;

    switch (choice) {
    case 1: it->setStatus(OrderStatus::Pending); break;
    case 2: it->setStatus(OrderStatus::Shipped); break;
    case 3: it->setStatus(OrderStatus::Delivered); break;
    default:
        std::cerr << "❌ Invalid choice. Status not updated.\n";
        return;
    }

    saveToFile(); // Save updated order list
    std::cout << "✅ Order status updated successfully.\n";
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

    loadFromFile(); // 🔹 Зареди поръчките от файла

    switch (choice) {
    case 1: // Сортиране по рейтинг
        std::sort(orders.begin(), orders.end(), [](const Order& a, const Order& b) {
            double aRating = 0, bRating = 0;
            for (const auto& item : a.getItems()) aRating += item.first.getRating();
            for (const auto& item : b.getItems()) bRating += item.first.getRating();
            return aRating > bRating;
            });
        break;

    case 2: // Сортиране по цена (възходящо)
        std::sort(orders.begin(), orders.end(), [](const Order& a, const Order& b) {
            return a.getTotalPrice() < b.getTotalPrice();
            });
        break;

    case 3: // Сортиране по цена (низходящо)
        std::sort(orders.begin(), orders.end(), [](const Order& a, const Order& b) {
            return a.getTotalPrice() > b.getTotalPrice();
            });
        break;

    case 4: // По азбучен ред по първия артикул
        std::sort(orders.begin(), orders.end(), [](const Order& a, const Order& b) {
            if (a.getItems().empty() || b.getItems().empty()) return false;
            return a.getItems()[0].first.getName() < b.getItems()[0].first.getName();
            });
        break;

    default:
        std::cout << "Invalid choice. Showing unsorted list.\n";
        break;
    }

    for (const auto& order : orders) {
        order.display();
        std::cout << "-----------------------------\n";
    }
}
void Order::update(int id) {
    for (auto& order : orders) {
        if (order.getId() == id) {
            int choice;
            std::cout << "1. Pending\n2. Shipped\n3. Delivered\nSelect new status: ";
            std::cin >> choice;

            switch (choice) {
            case 1: order.setStatus(OrderStatus::Pending); break;
            case 2: order.setStatus(OrderStatus::Shipped); break;
            case 3: order.setStatus(OrderStatus::Delivered); break;
            default: std::cout << "Invalid status.\n"; return;
            }

           //the I saveToFile();
            std::cout << "Order updated.\n";
            return;
        }
    }
    std::cerr << "Order not found.\n";
}
void Order::loadFromFile() {
    std::ifstream inFile("Orders.txt");
    if (!inFile.is_open()) {
        std::cerr << "Cannot open Orders.txt\n";
        return;
    }

    orders.clear();             // 🧹 Изчистване на текущите поръчки
    nextId = 1;                 // 🔁 Рестарт на ID, ще се коригира от прочетените поръчки

    int orderId;
    std::string clientEGN, statusStr;
    int itemCount;
    double totalPrice, rewardPoints;

    while (inFile >> orderId) {
        inFile.ignore();  // skip newline
        std::getline(inFile, clientEGN);
        std::getline(inFile, statusStr);
        inFile >> itemCount;
        inFile.ignore();

        Order o;
        o.setId(orderId);               // 🔑 Правилно задаване на ID (също обновява nextId)
        o.setClientEGN(clientEGN);

        if (statusStr == "Pending") o.setStatus(OrderStatus::Pending);
        else if (statusStr == "Shipped") o.setStatus(OrderStatus::Shipped);
        else if (statusStr == "Delivered") o.setStatus(OrderStatus::Delivered);

        for (int i = 0; i < itemCount; ++i) {
            std::string itemName;
            double itemPrice;

            std::getline(inFile, itemName);
            inFile >> itemPrice;
            inFile.ignore();

            Item item;
            item.setName(itemName);
            item.setPrice(itemPrice);

            o.items.emplace_back(item, 1);  // добавяме с фиксирано количество 1
        }

        inFile >> totalPrice >> rewardPoints;
        inFile.ignore();

        if (findIndexById(orderId) == -1) {
            orders.push_back(o);
        }

        // ✅ Увери се, че nextId винаги надминава най-голямото ID
        if (orderId >= nextId) {
            nextId = orderId + 1;
        }
    }

    inFile.close();
}
void Order::display() const {
    std::cout << "Order ID: " << id << "\nStatus: " << statusToString(status) << "\nItems:\n";
    for (const auto& pair : items) {
        const Item& item = pair.first;
        int quantity = pair.second;
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
        std::cerr << "❌ Cannot open Reject.txt\n";
        return;
    }

    std::cout << "\n🛑 Rejected Orders:\n";

    int id;
    std::string egn, status, itemName, reason;
    int itemCount;
    double itemPrice, total, points;

    while (inFile >> id) {
        inFile.ignore(); // Skip newline
        std::getline(inFile, egn);
        std::getline(inFile, status);
        inFile >> itemCount;
        inFile.ignore();

        std::vector<std::pair<std::string, double>> items;

        for (int i = 0; i < itemCount; ++i) {
            std::getline(inFile, itemName);
            inFile >> itemPrice;
            inFile.ignore();
            items.emplace_back(itemName, itemPrice);
        }

        inFile >> total >> points;
        inFile.ignore();
        std::getline(inFile, reason);

        // Display
        std::cout << "\nOrder ID: " << id << "\n";
        std::cout << "Client EGN: " << egn << "\n";
        std::cout << "Status: " << status << "\n";
        std::cout << "Items:\n";
        for (const auto& p : items) {
            std::cout << "- " << p.first << " - " << p.second << " BGN\n";
        }
        std::cout << "Total Price: " << total << " BGN\n";
        std::cout << "Reward Points: " << points << "\n";
        std::cout << "❌ Rejection Reason: " << reason << "\n";
        std::cout << "----------------------------\n";
    }

    inFile.close();
}
void Order::rejectOrderById(int id, std::string reason) {
    loadFromFile();  // Увери се, че всички поръчки са заредени

    auto it = std::find_if(orders.begin(), orders.end(), [id](const Order& o) {
        return o.getId() == id;
        });

    if (it == orders.end()) {
        std::cerr << "❌ No order found with ID: " << id << "\n";
        return;
    }

    const Order& order = *it;

    std::ofstream outFile("Reject.txt", std::ios::app);
    if (!outFile.is_open()) {
        std::cerr << "❌ Failed to open Refunds.txt\n";
        return;
    }

    outFile << order.getId() << "\n";
    outFile << order.clientEGN << "\n";
    outFile << statusToString(order.getStatus()) << "\n";
    outFile << order.getItems().size() << "\n";

    for (const auto& pair : order.getItems()) {
        const Item& item = pair.first;
        outFile << item.getName() << "\n";
        outFile << item.getPrice() << "\n";
    }

    outFile << order.getTotalPrice() << "\n";
    outFile << order.getRewardPoints() << "\n";
    outFile << reason<< "\n";
    outFile.close();

    std::cout << "✅ Order ID " << id << " successfully written to Refunds.txt\n";
}
void Order::refundOrderById(int id) {
    loadFromFile();  // Увери се, че всички поръчки са заредени

    auto it = std::find_if(orders.begin(), orders.end(), [id](const Order& o) {
        return o.getId() == id;
        });

    if (it == orders.end()) {
        std::cerr << "❌ No order found with ID: " << id << "\n";
        return;
    }

    const Order& order = *it;

    std::ofstream outFile("Refunds.txt", std::ios::app);
    if (!outFile.is_open()) {
        std::cerr << "❌ Failed to open Refunds.txt\n";
        return;
    }

    outFile << order.getId() << "\n";
    outFile << order.clientEGN << "\n";
    outFile << statusToString(order.getStatus()) << "\n";
    outFile << order.getItems().size() << "\n";

    for (const auto& pair : order.getItems()) {
        const Item& item = pair.first;
        outFile << item.getName() << "\n";
        outFile << item.getPrice() << "\n";
    }

    outFile << order.getTotalPrice() << "\n";
    outFile << order.getRewardPoints() << "\n";
    outFile.close();

    std::cout << "✅ Order ID " << id << " successfully written to Refunds.txt\n";
}
void Order::displayPending() {
    loadFromFile();  // Зарежда всички поръчки от файла

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
    loadFromFile();  // Зарежда всички поръчки от файла

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
    loadFromFile();  // Зарежда всички поръчки от файла

    bool found = false;
    for (const auto& order : orders) {
        if (order.getStatus() == OrderStatus::Delivered) {
            std::cout << "-----------------------------\n";
            order.display();
            found = true;
        }
    }
}
void Order::displayUser(std::string eng) {
    loadFromFile();  // Зарежда всички поръчки от файла

    bool found = false;
    for (const auto& order : orders) {
        if (order.clientEGN == eng && order.getStatus() == OrderStatus::Shipped) {
            std::cout << "-----------------------------\n";
            order.display();
            found = true;
        }
    }
}
void Order::displayUserShipped(std::string eng) {
    loadFromFile();  // Зарежда всички поръчки от файла

    bool found = false;
    for (const auto& order : orders) {
        if (order.clientEGN == eng) {
            std::cout << "-----------------------------\n";
            order.display();
            found = true;
        }
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
