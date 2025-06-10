#pragma once
#include <string>

class Item; // ?? Forward declaration

class ItemCrud {
public:
    virtual void createItem() = 0;
    virtual Item read(int id) const = 0;
    virtual void update(int id) = 0;
    virtual void remove(int id) = 0;

    virtual ~ItemCrud() = default;
};
