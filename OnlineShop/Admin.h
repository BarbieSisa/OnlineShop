// Admin.h
#pragma once
#include <string>
#include "User.h"
class Admin : public User {
private:
    double totalSystemSum;
public:
    Admin();
    Admin(const char* name, const char* egn, const char* password, double totalSystemSum);

    double getTotalSystemSum() const;
    void setTotalSystemSum(double sum);

    

    static void sendCheck(double amount, const std::string& code, const std::string& clientEGN);
    static void customerInsights();
    static void viewTransactions();
   static void displayChecksByEgn(const std::string& targetEgn);
  static void displayAllChecks();
   static void createCheck(const std::string& code, double amount, const std::string& egn);
  static void removeCheckByCode(const std::string& targetCode);
  std::string loginAdminByUsernameAndPassword(const std::string& username, const std::string& password);
};