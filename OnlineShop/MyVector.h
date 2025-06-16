#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <cstddef>
#include <stdexcept>

template <typename T>
class MyVector {
private:
    T* data;
    size_t capacity;
    size_t count;

    void resize();

public:
    MyVector();
    ~MyVector();

    void push_back(const T& value);
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    size_t size() const;
    void clear();
};

#include "MyVector.tpp"  // Template implementation

#endif
