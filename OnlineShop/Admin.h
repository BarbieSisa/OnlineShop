#pragma once
#include "User.h"
#include "MyString.h"
#include "MyVector.hpp"

class Admin : public User {
private:
    double totalSystemSum;

public:
    Admin();
    Admin(const char* name, const char* egn, const char* password, double totalSystemSum);

    double getTotalSystemSum() const;
    void setTotalSystemSum(double sum);

    static void sendCheck(double amount, const MyString& code, const MyString& clientEGN);
    static void customerInsights();
    static void viewTransactions();
    static void displayChecksByEgn(const MyString& targetEgn);
    static void displayAllChecks();
    static void createCheck(const MyString& code, double amount, const MyString& egn);
    static void removeCheckByCode(const MyString& targetCode);

    MyString loginAdminByUsernameAndPassword(const MyString& username, const MyString& password);

    virtual ~Admin() override = default;
};
