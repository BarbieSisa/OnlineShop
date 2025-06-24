#include "Client.h"
#include "Item.h"
#include "Order.h"
#include "CustomerStat.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
int parseInt(const char* str) {
    int result = 0;
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        ++str;
    }
    return result;
}
double parseDouble(const char* str) {
    double result = 0.0, frac = 0.0;
    int divisor = 1;
    bool afterDecimal = false;

    while ((*str >= '0' && *str <= '9') || *str == '.') {
        if (*str == '.') {
            afterDecimal = true;
        }
        else if (!afterDecimal) {
            result = result * 10 + (*str - '0');
        }
        else {
            frac = frac * 10 + (*str - '0');
            divisor *= 10;
        }
        ++str;
    }

    return result + frac / divisor;
}
bool areEqualEGNs(const MyString& a, const MyString& b) {
    const char* sa = a.c_str();
    const char* sb = b.c_str();
    while (*sa && *sb) {
        if (*sa != *sb) return false;
        ++sa;
        ++sb;
    }
    return *sa == '\0' && *sb == '\0';
}
void Admin::customerInsights() {
    std::ifstream in("Orders.txt");
    if (!in.is_open()) {
        std::cout << "Cannot open Orders.txt\n";
        return;
    }

    char buffer[1024];
    MyVector<CustomerStat> stats;

    while (in.getline(buffer, sizeof(buffer))) {
        int orderId = parseInt(buffer);

        in.getline(buffer, sizeof(buffer));
        MyString egn(buffer);

        in.getline(buffer, sizeof(buffer)); // status
        in.getline(buffer, sizeof(buffer));
        int itemCount = parseInt(buffer);

        for (int i = 0; i < itemCount; ++i) {
            in.getline(buffer, sizeof(buffer)); // item name
            in.getline(buffer, sizeof(buffer)); // item price
        }

        in.getline(buffer, sizeof(buffer));
        double totalPrice = parseDouble(buffer);

        in.getline(buffer, sizeof(buffer)); // reward points

        bool found = false;
        for (size_t i = 0; i < stats.size(); ++i) {
            if (areEqualEGNs(stats[i].egn, egn)) {
                stats[i].orderCount++;
                stats[i].totalSpent += totalPrice;
                found = true;
                break;
            }
        }

        if (!found) {
            CustomerStat cs;
            cs.egn = egn;
            cs.orderCount = 1;
            cs.totalSpent = totalPrice;
            stats.push_back(cs);
        }
    }

    in.close();

    std::cout << "Customer Insights:\n";
    for (size_t i = 0; i < stats.size(); ++i) {
        std::cout << "EGN: " << stats[i].egn.c_str()
            << " | Orders: " << stats[i].orderCount
            << " | Total Spent: " 
            << stats[i].totalSpent << " BGN\n";
    }
}
void Admin::sendCheck(double amount, const MyString& code, const MyString& clientEGN) {
    std::ofstream outFile("Checks.txt", std::ios::app);
    if (!outFile.is_open()) {
        std::printf("Cannot open Checks.txt\n");
        return;
    }

    outFile << code.c_str() << "\n"
        << amount << "\n"
        << clientEGN.c_str() << "\n";

    outFile.close();

    std::printf("Check issued successfully for %.2f BGN to EGN: %s\n",
        amount, clientEGN.c_str());
}
void Admin::viewTransactions() {
    std::fstream checkFile("Checks.txt", std::ios::in);
    if (!checkFile.is_open()) {
        std::cout << "Cannot open Checks.txt\n";
        return;
    }

    char buffer[256];
    MyString code, amountStr, egn;
    double amount;

    std::cout << "All Transactions:\n";
    while (checkFile.getline(buffer, 256)) {
        code = buffer;

        if (!checkFile.getline(buffer, 256)) break;
        amountStr = buffer;

        if (!checkFile.getline(buffer, 256)) break;
        egn = buffer;

        amount = parseDouble(amountStr.c_str());

        std::cout << "Code: " << code
            << ", Amount: " << amount
            << ", Client EGN: " << egn << "\n";
    }

    checkFile.close();
}
void Admin::displayChecksByEgn(const MyString& targetEgn) {
    std::fstream file("Checks.txt", std::ios::in);
    if (!file.is_open()) {
        std::cout << "Cannot open Checks.txt\n";
        return;
    }

    char buffer[256];
    MyString code, amountStr, egn;
    double amount;
    bool found = false;

    std::cout << "\nTransactions for EGN: " << targetEgn << "\n";

    while (file.getline(buffer, 256)) {
        code = buffer;

        if (!file.getline(buffer, 256)) break;
        amountStr = buffer;

        if (!file.getline(buffer, 256)) break;
        egn = buffer;

        amount = parseDouble(amountStr.c_str());

        if (egn == targetEgn) {
            std::cout << "Code: " << code
                << ", Amount: " << amount
                << ", EGN: " << egn << "\n";
            found = true;
        }
    }

    if (!found) {
        std::cout << "No transactions found for this EGN.\n";
    }

    file.close();
}
void Admin::displayAllChecks() {
    std::fstream file("Checks.txt", std::ios::in);
    if (!file.is_open()) {
        std::cout << "Cannot open Checks.txt\n";
        return;
    }

    char buffer[256];
    MyString code, amountStr, egn;
    double amount;
    bool found = false;

    std::cout << "\nAll Transactions:\n";

    while (file.getline(buffer, 256)) {
        code = buffer;

        if (!file.getline(buffer, 256)) break;
        amountStr = buffer;
        amount = parseDouble(amountStr.c_str());  

        if (!file.getline(buffer, 256)) break;
        egn = buffer;

        std::cout << "Code: " << code.c_str()
            << ", Amount: " << amount
            << ", EGN: " << egn.c_str() << "\n";

        found = true;
    }

    if (!found) {
        std::cout << "No transactions found.\n";
    }
}
void Admin::createCheck(const MyString& code, double amount, const MyString& egn) {
    std::fstream checkFile("Checks.txt", std::ios::out | std::ios::app);
    if (!checkFile.is_open()) {
        std::cout << "Cannot open Checks.txt for writing.\n";
        return;
    }

    checkFile << code.c_str() << '\n';
    checkFile << amount << '\n';
    checkFile << egn.c_str() << '\n';

    std::cout << "Transaction recorded: Code=" << code.c_str()
        << ", Amount=" << amount
        << ", EGN=" << egn.c_str() << "\n";
}
 void Admin::removeCheckByCode(const MyString& targetCode) {
     std::fstream inFile("Checks.txt", std::ios::in);
     if (!inFile.is_open()) {
         std::cout << "Cannot open Checks.txt\n";
         return;
     }

     MyVector<MyString> lines;
     char buffer[128];
     char code[128], line1[128], line2[128];
     bool removed = false;

     while (inFile.getline(code, 128)) {
         if (!inFile.getline(line1, 128) || !inFile.getline(line2, 128)) {
             break;
         }

         MyString readCode(code);
         if (readCode == targetCode) {
             removed = true;
         }
         else {
             lines.push_back(MyString(code));
             lines.push_back(MyString(line1));
             lines.push_back(MyString(line2));
         }
     }
     inFile.close();

     std::fstream outFile("Checks.txt", std::ios::out | std::ios::trunc);
     if (!outFile.is_open()) {
         std::cout << "Cannot open Checks.txt for writing\n";
         return;
     }

     for (size_t i = 0; i < lines.size(); ++i) {
         outFile << lines[i].c_str() << '\n';
     }

     if (removed) {
         std::cout << "Transaction with code \"" << targetCode.c_str() << "\" was removed.\n";
     }
     else {
         std::cout << "No transaction with code \"" << targetCode.c_str() << "\" was found.\n";
     }
 }
 MyString Admin::loginAdminByUsernameAndPassword(const MyString& username, const MyString& password) {
     std::fstream inFile("Admins.txt", std::ios::in);
     if (!inFile.is_open()) {
         std::cout << "Cannot open Admins.txt\n";
         return "invalid";
     }

     char nameBuffer[128];
     char egnBuffer[16];
     char passwordBuffer[128];
     double totalSystemSum;

     while (inFile.getline(nameBuffer, 128) &&
         inFile.getline(egnBuffer, 16) &&
         inFile.getline(passwordBuffer, 128) &&
         inFile >> totalSystemSum) {
         inFile.ignore();

         MyString fileName(nameBuffer);
         MyString fileEgn(egnBuffer);
         MyString filePassword(passwordBuffer);

         if (fileName == username && filePassword == password) {
             return fileEgn;
         }
     }

     return "invalid";
 }