// OrderCrud.h
#pragma once
#include "Item.h"



class Order; // Forward declaration

class OrderCrud {
public:
    virtual void createOrder() = 0;
    virtual Order read(int id) const = 0;
    virtual void update(int id) = 0;
    virtual void remove(int id) = 0;

    virtual ~OrderCrud() = default;
};

