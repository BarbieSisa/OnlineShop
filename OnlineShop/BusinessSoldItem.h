#pragma once
#include "Item.h"

struct BusinessSoldItem {
    int quantity;
    Item item;

    BusinessSoldItem() : quantity(0) {}
    BusinessSoldItem(const Item& item, int quantity) : item(item), quantity(quantity) {}

    void set(const Item& newItem, int newQuantity) {
        item = newItem;
        quantity = newQuantity;
    }

    const Item& getItem() const {
        return item;
    }

    int getQuantity() const {
        return quantity;
    }
};