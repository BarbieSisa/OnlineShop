#include "Client.h"
#include "Order.h"
#include "Item.h" // If needed for loadFromFile()
#include <algorithm> 
#include <vector>
#include <iostream>

int main() {
    // Simulate a logged-in client
    Client client("Sofia Raffaelli", "0987654321", "rafaelli", 100);
    std::string reason;
    // Vector of items to be added to current order
    std::vector<Item> currentItems;
    bool discountApplied;

    // Load items from Items.txt first
    std::cout << "🔄 Loading available items...\n";

    try {
        // Optional: load from file or check by reading IDs manually
        // This just ensures Items.txt exists and is readable
        Item test = Item::read(1); // remove or wrap in loop later
        std::cout << "✅ Item database loaded.\n";
    }
    catch (...) {
        std::cout << "⚠️ Warning: Could not load items. Make sure Items.txt exists and has valid entries.\n";
    }

    int choice;
    do {
        std::cout << "\n===== Order Menu =====\n";
        std::cout << "1. Add Item to Order\n";
        std::cout << "2. Remove Item from Order\n";
        std::cout << "3. Checkout Order\n";
        std::cout << "0. Exit\n";
        std::cout << "=======================\n";
        std::cout << "Choice: ";
        std::cin >> choice;


        switch (choice) {
        case 1:
            Order::addItemToOrder(currentItems);  // static method
            break;
        case 2:
            Order::removeItemFromOrder(currentItems);  // static method
            break;
        case 3:
            Order::applyDiscount(client.getEGN(),currentItems, discountApplied);
            Order::viewCart(currentItems, discountApplied);
           // Order::removeDiscount(client.getEGN(), currentItems, discountApplied);
            Order::checkoutOrder(currentItems, client.getEGN());  // writes to Orders.txt
            currentItems.clear(); // Start new order after checkout
            break;
        case 4:
            
            Order::displayPending(); 
            int orderID;
            std::cin >> orderID;
            Order::updateOrderStatusById(orderID); 
            // writes to Orders.txt
            currentItems.clear(); // Start new order after checkout
            break;
        case 5:
            int id;

            std::cin >> id;
            
            Order::rejectOrderById(id,"I reject this order");
            Order::displayRejectedOrders();// writes to Orders.txt
           // currentItems.clear(); // Start new order after checkout
            break;
        case 6:
           // int id;
         //  ;
            //std::cin >> id;
            double amount;
            double points;
            std::cout << "Enter amount to add to balance: ";
            std::cin >> amount;

            std::cout << "Enter points to add: ";
            std::cin >> points;
           // Client::updateBalanceForOrder(client.getEGN(), amount);
            Client::updatePoints(client.getEGN(), points);
           // Order::displayUser("1234567890");// writes to Orders.txt
            // currentItems.clear(); // Start new order after checkout
            break;
        case 7:
            // int id;
          //  ;
             //std::cin >> id;

            Client::create();// writes to Orders.txt
            // currentItems.clear(); // Start new order after checkout
            break;
        case 0:
            std::cout << "👋 Exiting...\n";
            break;
        default:
            std::cout << "❌ Invalid choice. Try again.\n";
            break;
        }

    } while (choice != 0);

    return 0;
}
