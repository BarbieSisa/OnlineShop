#include "Client.h"
#include "Item.h"
#include "Order.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

MyVector<Client> Client::clients;
Client::Client(const char* name, const char* egn, const char* password, double balance)
    : User(name, egn, password), balance(balance), points(0) {
}
int Client::findIndexByEGN(const MyString& egn) {
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i].getEGN() == egn) {
            return static_cast<int>(i);
        }
    }
    return -1;
}
void Client::updateBalanceForOrder(const MyString& egn, double amount) {
    int index = findIndexByEGN(egn);
    if (index == -1) {
        std::cout << "Client with EGN " << egn.c_str() << " not found.\n";
        return;
    }

    clients[index].balance += amount;
    saveAllClientsToFile();

    std::cout << "Balance updated. New balance: " << clients[index].balance << "\n";
}
void Client::setPoints(int p) {
    points = p;
}
void Client::updateBalance(const MyString& egn, double amount) {
    int index = findIndexByEGN(egn);
    if (index == -1) {
        std::cout << "Client with EGN " << egn.c_str() << " not found.\n";
        return;
    }

    clients[index].balance -= amount;
    saveAllClientsToFile();

    std::cout << "Balance updated. New balance: " << clients[index].balance << "\n";
}
void Client::updatePoints(const MyString& egn, int points) {
    loadAllClientsFromFile();

    int index = findIndexByEGN(egn);
    if (index == -1) {
        std::cout << "Client with EGN " << egn.c_str() << " not found.\n";
        return;
    }

    clients[index].points += static_cast<int>(points);

    saveAllClientsToFile();

    std::cout << "Points updated. New points: " << clients[index].points << "\n";
}
void Client::saveAllClientsToFile() {
    std::ofstream outFile("Clients.txt");
    if (!outFile) {
        std::cerr << "❌ Failed to open Clients.txt for writing.\n";
        return;
    }

    for (const auto& client : clients) {
        outFile << client.getName() << '\n'
            << client.getEGN() << '\n'
            << client.getPassword() << '\n'
            << client.getBalance() << '\n'
            << client.getPoints() << '\n';
    }

    outFile.close();
}
void Client::loadAllClientsFromFile() {
    std::ifstream inFile("Clients.txt");
    if (!inFile) {
        std::cout << "Cannot open Clients.txt\n";
        return;
    }

    clients.clear();

    char buffer[256];
    while (inFile.getline(buffer, sizeof(buffer))) {
        MyString name(buffer);
        inFile.getline(buffer, sizeof(buffer));
        MyString egn(buffer);
        inFile.getline(buffer, sizeof(buffer));
        MyString password(buffer);

        double balance = 0;
        int points = 0;

        inFile >> balance >> points;
        inFile.ignore();

        Client client(name.c_str(), egn.c_str(), password.c_str(), balance);
        client.setPoints(points);

        clients.push_back(client);
    }

    inFile.close();
    std::cout << "Clients loaded successfully. Total: " << clients.size() << "\n";
}
void Client::create() {
    char nameBuffer[256], egnBuffer[20], passwordBuffer[256], balanceBuffer[50];
    double balance;

    std::cin.ignore(10000, '\n'); 

    std::cout << "Enter name: ";
    std::cin.getline(nameBuffer, sizeof(nameBuffer));

    std::cout << "Enter EGN: ";
    std::cin.getline(egnBuffer, sizeof(egnBuffer));

    std::cout << "Enter password: ";
    std::cin.getline(passwordBuffer, sizeof(passwordBuffer));

    std::cout << "Enter initial balance: ";
    std::cin.getline(balanceBuffer, sizeof(balanceBuffer));
    balance = toDouble(balanceBuffer);  // твоя имплементация или std::atof

    MyString name(nameBuffer);
    MyString egn(egnBuffer);
    MyString password(passwordBuffer);

    Client newClient(name.c_str(), egn.c_str(), password.c_str(), balance);
    clients.push_back(newClient);

    std::ofstream outFile("Clients.txt", std::ios::app);
    if (outFile) {
        outFile << name.c_str() << '\n'
            << egn.c_str() << '\n'
            << password.c_str() << '\n'
            << balance << '\n'
            << 0 << '\n';  // Начални точки
        std::cout << "Client registered successfully.\n";
    }
    else {
        std::cerr << "Failed to open file for writing.\n";
    }
}
int Client::toInt(const char* str) {
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
double Client::toDouble(const char* str) {
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
void Client::approveCheck(const MyString& code, const MyString& egn) {
    std::ifstream checkFile("Checks.txt");
    if (!checkFile) {
        std::cerr << "Cannot open Checks.txt\n";
        return;
    }

    char buffer[256];
    MyString checkCode, lineAmount, checkEGN;
    double amount = 0;
    bool found = false;

    MyVector<MyString> updatedChecks;

    while (checkFile.getline(buffer, sizeof(buffer))) {
        checkCode = buffer;

        if (!checkFile.getline(buffer, sizeof(buffer))) break;
        lineAmount = buffer;

        if (!checkFile.getline(buffer, sizeof(buffer))) break;
        checkEGN = buffer;

        if (checkCode == code && checkEGN == egn) {
            amount = toDouble(lineAmount.c_str());
            found = true;
            continue; // skip writing this one
        }

        updatedChecks.push_back(checkCode);
        updatedChecks.push_back(lineAmount);
        updatedChecks.push_back(checkEGN);
    }
    checkFile.close();

    if (!found) {
        std::cout << "No matching check with code \"" << code.c_str()
            << "\" for EGN \"" << egn.c_str() << "\".\n";
        return;
    }

    loadAllClientsFromFile();
    updateBalanceForOrder(egn.c_str(), amount);

    std::ofstream outFile("Checks.txt", std::ios::trunc);
    for (size_t i = 0; i < updatedChecks.size(); ++i) {
        outFile << updatedChecks[i].c_str() << '\n';
    }

    std::cout << "Check approved. " << amount << " added to balance of client with EGN "
        << egn.c_str() << ".\n";

    Admin::removeCheckByCode(code);
}
void Client::addOrder(const Order& order) {
    orders.push_back(order);
}
MyString Client::getEGN() const {
    return User::getEGN();  
}
void Client::logout() const {
    std::cout << "Logged out.\n";
}
void Client::help() const {
    std::cout << "Client Help Section\n";
}
void Client::viewProfile() const {
    std::cout << "Name: " << getName() << "\nEGN: " << getEGN() << "\nBalance: " << balance << "\nPoints: " << getPoints() << "\n";
}
