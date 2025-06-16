#include "Admin.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <unordered_map>

void Admin::sendCheck(double amount, const std::string& code, const std::string& clientEGN) {
    std::ofstream outFile("Checks.txt", std::ios::app);
    if (!outFile) {
        std::cerr << "❌ Cannot open Checks.txt\n";
        return;
    }
    outFile << code << "\n" << amount << "\n" << clientEGN << "\n";
    outFile.close();
    std::cout << "✅ Check issued successfully for " << amount << " BGN to EGN: " << clientEGN << "\n";
}

void Admin::customerInsights() {
    std::ifstream orderFile("Orders.txt");
    if (!orderFile) {
        std::cerr << "❌ Cannot open Orders.txt\n";
        return;
    }

    std::unordered_map<std::string, int> orderCount;
    std::unordered_map<std::string, double> totalSpent;

    std::string egn, status;
    int orderId, itemCount;
    double price, rewardPoints;
    std::string line;

    while (orderFile >> orderId) {
        orderFile.ignore();
        std::getline(orderFile, egn);
        std::getline(orderFile, status);
        orderFile >> itemCount;
        orderFile.ignore();

        for (int i = 0; i < itemCount; ++i) {
            std::getline(orderFile, line); // name
            std::getline(orderFile, line); // price
            std::getline(orderFile, line); // available
        }

        orderFile >> price >> rewardPoints;
        orderFile.ignore();

        orderCount[egn]++;
        totalSpent[egn] += price;
    }

    std::cout << "📊 Customer Insights:\n";
    for (const auto& pair : orderCount) {
        std::cout << "EGN: " << pair.first
            << " | Orders: " << pair.second
            << " | Total Spent: " << std::fixed << std::setprecision(2) << totalSpent[pair.first] << " BGN\n";
    }
}

void Admin::viewTransactions() {
    std::ifstream checkFile("Checks.txt");
    if (!checkFile) {
        std::cerr << "❌ Cannot open Checks.txt\n";
        return;
    }
    std::string code, line;
    double amount;
    std::string egn;

    std::cout << "💸 All Transactions:\n";
    while (std::getline(checkFile, code)) {
        std::getline(checkFile, line);
        amount = std::stod(line);
        std::getline(checkFile, egn);
        std::cout << "Code: " << code << ", Amount: " << amount << ", Client EGN: " << egn << "\n";
    }
}
void Admin::displayChecksByEgn(const std::string& targetEgn) {
    std::ifstream file("Checks.txt");
    if (!file) {
        std::cerr << "❌ Cannot open Checks.txt\n";
        return;
    }

    std::string code, line, egn;
    double amount;
    bool found = false;

    std::cout << "\n💳 Transactions for EGN: " << targetEgn << "\n";

    while (std::getline(file, code)) {
        if (!std::getline(file, line) || !std::getline(file, egn)) {
            std::cerr << "⚠️ Incomplete transaction. Skipping.\n";
            break;
        }

        amount = std::stod(line);
        if (egn == targetEgn) {
            std::cout << "Code: " << code
                << ", Amount: " << amount
                << ", EGN: " << egn << "\n";
            found = true;
        }
    }

    if (!found) {
        std::cout << "❗ No transactions found for this EGN.\n";
    }
}
void Admin::displayAllChecks() {
    std::ifstream file("Checks.txt");
    if (!file) {
        std::cerr << "Cannot open Checks.txt\n";
        return;
    }

    std::string code, line, egn;
    double amount;
    bool found = false;

    std::cout << "\nAll Transactions:\n";

    while (std::getline(file, code)) {
        if (!std::getline(file, line) || !std::getline(file, egn)) {
            std::cerr << "Incomplete transaction record. Skipping.\n";
            break;
        }

        amount = std::stod(line);
        std::cout << "Code: " << code
            << ", Amount: " << amount
            << ", EGN: " << egn << "\n";
        found = true;
    }

    if (!found) {
        std::cout << "❗ No transactions found.\n";
    }
}
 void Admin::createCheck(const std::string& code, double amount, const std::string& egn) {
    std::ofstream checkFile("Checks.txt", std::ios::app); // append mode
    if (!checkFile) {
        std::cerr << "❌ Cannot open Checks.txt for writing.\n";
        return;
    }

    checkFile << code << '\n'
        << amount << '\n'
        << egn << '\n';

    std::cout << "Transaction recorded: Code=" << code
        << ", Amount=" << amount
        << ", EGN=" << egn << "\n";
}
 void Admin::removeCheckByCode(const std::string& targetCode) {
     std::ifstream inFile("Checks.txt");
     if (!inFile) {
         std::cerr << "Cannot open Checks.txt\n";
         return;
     }

     std::vector<std::string> lines;
     std::string code, line1, line2;

     bool removed = false;

     while (std::getline(inFile, code)) {
         if (!std::getline(inFile, line1) || !std::getline(inFile, line2)) {
             std::cerr << "⚠️ Incomplete record found. Skipping.\n";
             break;
         }

         if (code == targetCode) {
             removed = true; // пропускаме тази транзакция
         }
         else {
             lines.push_back(code);
             lines.push_back(line1);
             lines.push_back(line2);
         }
     }
     inFile.close();

     std::ofstream outFile("Checks.txt", std::ios::trunc); // изтрива съдържанието
     if (!outFile) {
         std::cerr << "Cannot open Checks.txt for writing.\n";
         return;
     }

     for (const std::string& line : lines) {
         outFile << line << '\n';
     }

     if (removed) {
         std::cout << "Transaction with code \"" << targetCode << "\" was removed.\n";
     }
     else {
         std::cout << "No transaction with code \"" << targetCode << "\" was found.\n";
     }
 }
 std::string Admin::loginAdminByUsernameAndPassword(const std::string& username, const std::string& password) {
     std::ifstream inFile("Admins.txt");
     if (!inFile) {
         std::cerr << "❌ Cannot open Admins.txt\n";
         return "invalid";
     }

     std::string name, egn, filePassword;
     double totalSystemSum;

     while (std::getline(inFile, name) &&
         std::getline(inFile, egn) &&
         std::getline(inFile, filePassword) &&
         inFile >> totalSystemSum) {
         inFile.ignore(); // skip newline after totalSystemSum

         if (name == username && filePassword == password) {
             return egn; // ✅ успешен вход
         }
     }

     return "invalid"; // ❌ неуспешен вход
 }
