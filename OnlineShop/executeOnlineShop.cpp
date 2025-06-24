#include"Order.h"
#include "Client.h"
#include <iostream>
#include "Item.h"
#include"Admin.h"
#include "BusinessAccount.h"
#include "MyString.h"
#include "MyVector.hpp"
void businessMenu() {
    int choice;
    MyString username;
    MyString pass;
    char buffer[256];

    std::cout << "Username:\n";
    std::cin.getline(buffer, 256);
    username = buffer;

    std::cout << "Password:\n";
    std::cin.getline(buffer, 256);
    pass = buffer;

    BusinessAccount business;
    MyString egn = business.loginBusinessByUsernameAndPassword(username, pass);

    do {
        std::cout << "\n======= \u0411\u0438\u0437\u043d\u0435\u0441 \u041c\u0435\u043d\u044e =======\n";
        std::cout << "1. Add Item\n";
        std::cout << "2. Remove Item\n";
        std::cout << "3. List Pending Orders\n";
        std::cout << "4. Approve Order\n";
        std::cout << "5. Reject Order\n";
        std::cout << "6. List All Orders\n";
        std::cout << "7. List Best Selling Products\n";
        std::cout << "8. View Revenue\n";
        std::cout << "9. List Refund Requests\n";
        std::cout << "10. Approve Refund\n";
        std::cout << "11. Reject Refund\n";
        std::cout << "0. Exit\n";
        std::cout << "Choice: ";

        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
        case 1:
            Item::create();
            break;
        case 2: {
            int itemId;
            std::cout << "Type the ID of the item you want to remove: ";
            std::cin >> itemId;
            Item::removeById(itemId);
            break;
        }
        case 3:
            Order::displayPending();
            break;
        case 4: {
            int orderUpdateStatus;
            std::cout << "Enter order index to approve: ";
            std::cin >> orderUpdateStatus;
            Order::updateOrderStatusById(orderUpdateStatus);
            break;
        }
        case 5: {
            int orderRejectId;
            std::cout << "Enter order index to reject: ";
            std::cin >> orderRejectId;
            std::cin.ignore();
            std::cout << "Enter rejection reason: ";
            std::cin.getline(buffer, 256);
            MyString reason = buffer;
            Order::rejectOrderById(orderRejectId, reason);
            Order::refundOrderById(orderRejectId);
            break;
        }
        case 6:
            Order::displayAllOrdersFromFile();
            break;
        case 7:
            // OrderCrud::listBestSellingProducts();
            break;
        case 8:
            // OrderCrud::viewRevenue();
            break;
        case 9: {
            Order::displayRefundedOrders();
            std::cout << "Type the ID of the order you will approve refund to: ";
            int idOrder;
            std::cin >> idOrder;
            Order::approveRefund(idOrder);
            Order::refundOrderById(idOrder);
            break;
        }
        case 10: {
            int orderRejectIdRefund;
            std::cout << "Enter order index to reject: ";
            std::cin >> orderRejectIdRefund;
            std::cin.ignore();
            std::cout << "Enter rejection reason: ";
            std::cin.getline(buffer, 256);
            MyString reasonRefund = buffer;
            Order::rejectRefundOrderById(orderRejectIdRefund, reasonRefund);
            break;
        }
        case 11:
            std::cout << "Logout\n";
            execute();
            break;
        case 0:
            std::cout << "Logging out...\n";
            break;
        default:
            std::cout << "Invalid choice!\n";
            break;
        }

    } while (choice != 0);
}
void showClientMenu() {
    std::cout << "\n=========== CLIENT MENU ===========\n";
    std::cout << "1. Check balance\n";
    std::cout << "2. Redeem check\n";
    std::cout << "3. List products by rating\n";
    std::cout << "4. List products by price (asc)\n";
    std::cout << "5. List products by price (desc)\n";
    std::cout << "6. List products alphabetically\n";
    std::cout << "7. View product by ID\n";
    std::cout << "8. Add to cart\n";
    std::cout << "9. Remove from cart\n";
    std::cout << "10. Apply discount\n";
    std::cout << "11. Remove discount\n";
    std::cout << "12. View cart\n";
    std::cout << "13. Checkout\n";
    std::cout << "14. List orders\n";
    std::cout << "15. Confirm order\n";
    std::cout << "16. Order history\n";
    std::cout << "17. Rate product\n";
    std::cout << "18. Request refund\n";
    std::cout << "19. Refunded orders\n";
    std::cout << "20. Log out";
    std::cout << "0. Exit\n";
}
void clientMenue()
{
    MyString username;
    MyString pass;

    std::cout << "Username:" << std::endl;
    char buffer[128];
    std::cin.getline(buffer, 128);
    username = buffer;

    std::cout << "Password:" << std::endl;
    std::cin.getline(buffer, 128);
    pass = buffer;

    int choice;
    int id;
    Client client;
    MyString egn = client.loginClientByUsernameAndPassword(username, pass);
    int index = client.findIndexByEGN(egn);

    std::cout << "[DEBUG] Logged in with EGN: ";
    std::cout << egn.c_str() << "\n";

    bool discountApplied = 0;
    MyVector<Item> cart;
    MyVector<int> quantities;
    MyString codeCheck;

    while (true) {
        showClientMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
        case 1:
            break;
        case 2:
            std::cout << "These are the checks sent from the admin, which one you want to update";
            Admin::displayChecksByEgn(egn);
            std::cin.getline(buffer, 128);
            codeCheck = buffer;
            Client::approveCheck(codeCheck, egn);
            break;
        case 3:
            Item::displayByRating();
            break;
        case 4:
            Item::displayByPriceAsc();
            break;
        case 5:
            Item::displayByPriceDesc();
            break;
        case 6:
            Item::displayByAlphabetical();
            break;
        case 7:
            Item::displayAllProducts();
            std::cout << "Choose product to view";
            std::cin >> id;
            Item::viewProductById(id);
            break;
        case 8:
            Order::addItemToOrder(cart, quantities);
            break;
        case 9:
            Order::removeItemFromOrder(cart, quantities);
            break;
        case 10:
            Order::applyDiscount(egn, cart, discountApplied);
            break;
        case 11:
            Order::removeDiscount(egn, discountApplied);
            break;
        case 12:
            Order::viewCart(cart, discountApplied);
            break;
        case 13:
            Order::checkoutOrder(cart, egn);
            break;
        case 14:
            Order::displayAllOrdersFromFile();
            break;
        case 15:
            int orderIdConfirmed;
            Order::displayUser("1234567890");
            std::cin >> orderIdConfirmed;
            Order::UpdatePointsWhenConfirmed(orderIdConfirmed);
            break;
        case 16:
            Order::displayUserDelivered("1234567890");
            break;
        case 17:
            double newRating;
            std::cout << "Give a rating of a product.";
            Item::displayAllProducts();
            std::cout << "Type the ID of the product you want to rate";
            std::cin >> newRating;
            int itemId;
            std::cin >> itemId;
            Item::updateRating(itemId, newRating);
            break;
        case 18:
            std::cout << "Choose order to refund";
            Order::displayUser("0987654321");
            int orderId;
            std::cin >> orderId;
            Order::refundOrderById(orderId);
            break;
        case 19:
            Order::displayRefundedOrdersUsers(egn);
            break;
        case 20:
            std::cout << "Logout";
            execute();
            break;
        case 0:
            std::cout << "Goodbye!\n";
            return;
        default:
            std::cout << "Invalid choice. Try again.\n";
        }
    }
}
void login() {
    std::cout << "============ WELCOME TO THE SYSTEM ============\n";
    std::cout << "Please choose your user role to log in:\n";
    std::cout << "1. Client\n";
    std::cout << "2. BusinessAccount\n";
    std::cout << "3. Admin\n";
    std::cout << "===============================================\n";
    std::cout << "Enter role (Client/BusinessAccount/Admin): ";

    char input[64];
    std::cin >> input;
    MyString role(input);

    if (role == "Client" || role == "client") {
        std::cout << "\nLogging in as Client...\n";
        clientMenue();
    }
    else if (role == "BusinessAccount" || role == "business" || role == "Business") {
        std::cout << "\nLogging in as Business Account...\n";
        businessMenu();
    }
    else if (role == "Admin" || role == "admin") {
        std::cout << "\nLogging in as Admin...\n";
        adminMenu();
    }
    else {
        std::cout << "Invalid role. Please type 'Client', 'BusinessAccount' or 'Admin'.\n";
    }
}
void registerUser() {
    char input[64];
    std::cout << "\nUser Registration\n";
    std::cout << "Choose role to register as:\n";
    std::cout << "1. Client\n";
    std::cout << "2. Business\n";
    std::cout << "Enter role (client/business): ";
    std::cin >> input;
    for (int i = 0; input[i] != '\0'; ++i) {
        input[i] = std::tolower(input[i]);
    }

    MyString userRole(input);

    if (userRole == "client") {
        Client::create();
    }
    else if (userRole == "business") {
        BusinessAccount::create();
    }
    else {
        std::cout << "Invalid role. Please enter 'client' or 'business'.\n";
    }

    std::cout << "Now please login into the system\n";
    login();
}
void execute() {
    char input[64];

    while (true) {
        std::cout << "\n=== Welcome to the Online Shop ===\n";
        std::cout << "Please choose an option:\n";
        std::cout << "1. Register\n";
        std::cout << "2. Login\n";
        std::cout << "3. Exit\n";
        std::cout << "> ";

        std::cin.getline(input, 64);
        MyString choice(input);

        if (choice == "1" || choice == "register") {
            registerUser();
        }
        else if (choice == "2" || choice == "login") {
            login();
        }
        else if (choice == "0" || choice == "exit") {
            std::cout << "Thank you for visiting! Goodbye.\n";
            break;
        }
        else {
            std::cout << "Invalid option. Please try again.\n";
        }
    }
}
void adminMenu() {
    int choice;
    char usernameBuffer[128];
    char passwordBuffer[128];

    std::cout << "Admin Username: ";
    std::cin.getline(usernameBuffer, 128);
    MyString username(usernameBuffer);

    std::cout << "Admin Password: ";
    std::cin.getline(passwordBuffer, 128);
    MyString password(passwordBuffer);

    Admin admin;
    MyString adminEGN = admin.loginAdminByUsernameAndPassword(username, password);

    do {
        std::cout << "\n======= ADMIN MENU =======\n";
        std::cout << "1. Send check\n";
        std::cout << "2. Customer insights\n";
        std::cout << "3. View transactions\n";
        std::cout << "0. Logout\n";
        std::cout << "Choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
        case 1: {
            double amount;
            char codeBuffer[64];
            char egnBuffer[32];

            std::cout << "Amount: ";
            std::cin >> amount;
            std::cin.ignore();

            std::cout << "Code: ";
            std::cin.getline(codeBuffer, 64);
            MyString code(codeBuffer);

            std::cout << "EGN: ";
            std::cin.getline(egnBuffer, 32);
            MyString clientEGN(egnBuffer);

            Admin::sendCheck(amount, code, clientEGN);
            break;
        }
        case 2: {
            char egnBuffer[32];
            std::cout << "EGN: ";
            std::cin.getline(egnBuffer, 32);
            MyString targetEgn(egnBuffer);

            Order::displayUserSum(targetEgn);
            break;
        }
        case 3:
            Admin::displayAllChecks();
            break;
        case 0:
            std::cout << "Logging out...\n";
            execute();
            break;
        default:
            std::cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 0);
}
