#pragma once
#include "User.h"
#include "Item.h"
#include <utility>   // for std::pair>

class BusinessAccount : public User {
    std::pair<int, Item> soldItem;  // only one item and its count

public:
    BusinessAccount() = default;
  //  BusinessAccount(const char* name, const char* egn, const char* password, const Item& item, int count);

    BusinessAccount(const std::string& name, const std::string& egn,
        const std::string& password);

    void setSoldItem(const Item& item, int count);
    std::string loginBusinessByUsernameAndPassword(const std::string& username, const std::string& password);
    const std::pair<int, Item>& getSoldItem() const;
    void viewSoldItem() const;

    void viewProfile() const override;
    void help() const override;
   static void create();
  static void updateSoldQuantityInFile(int itemId, int quantityToAdd);
};