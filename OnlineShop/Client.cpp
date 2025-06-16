#include "Client.h"
#include "Item.h"
#include "Order.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

std::vector<Client> Client::clients;
Client::Client(const char* name, const char* egn, const char* password, double balance)
    : User(name, egn, password), balance(balance), points(0) {
}
int Client::findIndexByEGN(const std::string& egn) {
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i].getEGN() == egn) return static_cast<int>(i);
    }
    return -1;
}


void Client::updateBalanceForOrder(const std::string& egn, double amount) {
    int index = findIndexByEGN(egn);  // This must also be static
    if (index == -1) {
        std::cerr << "❌ Client with EGN " << egn << " not found.\n";
        return;
    }

    clients[index].balance += amount;
    saveAllClientsToFile();
    std::cout << "✅ Balance updated. New balance: " << clients[index].balance << "\n";
}
void Client::setPoints(int p) {
    points = p;
}

void Client::updateBalance(const std::string& egn, double amount) {
   
    int index = findIndexByEGN(egn);  // This must also be static
    if (index == -1) {
        std::cerr << "❌ Client with EGN " << egn << " not found.\n";
        return;
    }

    clients[index].balance -= amount;
    saveAllClientsToFile();
    std::cout << "✅ Balance updated. New balance: " << clients[index].balance << "\n";
}
void Client::loadAllClientsFromFile() {
    std::ifstream inFile("Clients.txt");
    if (!inFile) {
        std::cerr << "❌ Cannot open Clients.txt\n";
        return;
    }

    clients.clear();  // Изчистване на текущия вектор

    std::string name, egn, password;
    double balance;
    int points;

    while (std::getline(inFile, name) &&
        std::getline(inFile, egn) &&
        std::getline(inFile, password) &&
        inFile >> balance &&
        inFile >> points) {
        inFile.ignore();  // Прескачаме нов ред след числата

        Client client(name.c_str(), egn.c_str(), password.c_str(), balance);
        client.setPoints(points);  // Ако имаш setter
        clients.push_back(client);
    }

    inFile.close();
    std::cout << "✅ Clients loaded successfully. Total: " << clients.size() << "\n";
}

void Client::updatePoints(const std::string& egn, double points) {
    loadAllClientsFromFile();
    int index = findIndexByEGN(egn);  // This must also be static
    if (index == -1) {
        std::cerr << "❌ Client with EGN " << egn << " not found.\n";
        return;
    }

    clients[index].points += points;
    saveAllClientsToFile();
    std::cout << "✅ Points updated. New points: " << clients[index].points << "\n";
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
std::string Client::loginClientByUsernameAndPassword(const std::string& username, const std::string& password) {
    std::ifstream inFile("Clients.txt");
    if (!inFile) {
        std::cerr << "❌ Cannot open Clients.txt\n";
        return "invalid";
    }

    std::string name, egn, filePassword;
    double balance;
    int points;

    while (std::getline(inFile, name) &&
        std::getline(inFile, egn) &&
        std::getline(inFile, filePassword) &&
        inFile >> balance &&
        inFile >> points) {
        inFile.ignore(); // Прескачане на празен ред/нов ред след числата

        // 🔍 Проверка дали въведените username и password съвпадат
        if (name == username && filePassword == password) {
            return egn; // ✅ Успешен вход
        }
    }

    return "invalid"; // ❌ Няма съвпадение
}

void Client::create() {
    std::string name, egn, password;
    double balance;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter name: ";
    std::getline(std::cin, name);

    std::cout << "Enter EGN: ";
    std::getline(std::cin, egn);

    std::cout << "Enter password: ";
    std::getline(std::cin, password);

    std::cout << "Enter initial balance: ";
    std::cin >> balance;
    std::cin.ignore(); // Изчистване на \n след cin

    Client newClient(name.c_str(), egn.c_str(), password.c_str(), balance);
    clients.push_back(newClient);

    std::ofstream outFile("Clients.txt", std::ios::app);
    if (outFile) {
        outFile << name << '\n'
            << egn << '\n'
            << password << '\n'
            << balance << '\n'
            << 0 << '\n';  // Points start at 0
        std::cout << "Client registered successfully.\n";
    }
    else {
        std::cerr << "Failed to open file for writing.\n";
    }
}

void Client::approveCheck(const std::string& code, const std::string& egn) {
    std::ifstream checkFile("Checks.txt");
    if (!checkFile) {
        std::cerr << "❌ Cannot open Checks.txt\n";
        return;
    }

    std::string checkCode, lineAmount, checkEGN;
    double amount = 0;
    bool found = false;

    std::vector<std::string> updatedChecks;

    while (std::getline(checkFile, checkCode)) {
        if (!std::getline(checkFile, lineAmount) || !std::getline(checkFile, checkEGN)) {
            std::cerr << "⚠️ Incomplete check entry. Skipping.\n";
            break;
        }

        if (checkCode == code && checkEGN == egn) {
            amount = std::stod(lineAmount);
            found = true;
            // пропускаме този чек (той ще бъде одобрен)
        }
        else {
            // запазваме останалите чекове
            updatedChecks.push_back(checkCode);
            updatedChecks.push_back(lineAmount);
            updatedChecks.push_back(checkEGN);
        }
    }
    checkFile.close();

    if (!found) {
        std::cout << "❗ No matching check with code \"" << code << "\" for EGN \"" << egn << "\".\n";
        return;
    }

    // Обнови баланса чрез Client методите
    loadAllClientsFromFile();  // статичен метод, зарежда clients
    updateBalanceForOrder(egn, amount);  // обновява баланса и записва файла

    // Презаписваме Checks.txt с останалите
    std::ofstream outFile("Checks.txt", std::ios::trunc);
    for (const std::string& line : updatedChecks) {
        outFile << line << '\n';
    }

    std::cout << "✅ Check approved. " << amount << " added to balance of client with EGN " << egn << ".\n";
    Admin::removeCheckByCode(code);
}


void Client::addOrder(const Order& order) {
    orders.push_back(order);
}




std::string Client::getEGN() const {
    return std::string(User::getEGN());
    
}







// Placeholder implementations for inherited virtuals
void Client::logout() const {
    std::cout << "Logged out.\n";
}

void Client::help() const {
    std::cout << "Client Help Section\n";
}

void Client::viewProfile() const {
    std::cout << "Name: " << getName() << "\nEGN: " << getEGN() << "\nBalance: " << balance << "\nPoints: " << getPoints() << "\n";
}
