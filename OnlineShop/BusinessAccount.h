#pragma once

#include "User.h"
#include "Item.h"
#include "MyString.h"
#include "MyVector.hpp"
#include "BusinessSoldItem.h"

class BusinessAccount : public User {
    BusinessSoldItem soldItem;
    MyVector<int> soldCounts;       

public:
    BusinessAccount();
    BusinessAccount(const MyString& name, const MyString& egn, const MyString& password);
    void addSoldItem(const Item& item, int count);
    const MyVector<Item>& getSoldItems() const;
    void setSoldItem(const Item& item, int count);
    const BusinessSoldItem& getSoldItem() const;
    MyString loginBusinessByUsernameAndPassword(const MyString& username, const MyString& password);
    void viewSoldItems() const;
    void viewProfile() const override;
    void help() const override;
    static void create();
    static void updateSoldQuantityInFile(int itemId, int quantityToAdd);
};
