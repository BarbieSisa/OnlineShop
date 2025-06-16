#pragma once

#include "MyVector.h"
#include <algorithm>

template <typename T>
MyVector<T>::MyVector() : data(nullptr), capacity(0), count(0) {
    resize();
}

template <typename T>
MyVector<T>::~MyVector() {
    delete[] data;
}

template <typename T>
void MyVector<T>::resize() {
    size_t newCapacity = (capacity == 0) ? 2 : capacity * 2;
    T* newData = new T[newCapacity];

    for (size_t i = 0; i < count; ++i)
        newData[i] = data[i];

    delete[] data;
    data = newData;
    capacity = newCapacity;
}

template <typename T>
void MyVector<T>::push_back(const T& value) {
    if (count >= capacity)
        resize();
    data[count++] = value;
}

template <typename T>
T& MyVector<T>::operator[](size_t index) {
    if (index >= count)
        throw std::out_of_range("Index out of bounds");
    return data[index];
}

template <typename T>
const T& MyVector<T>::operator[](size_t index) const {
    if (index >= count)
        throw std::out_of_range("Index out of bounds");
    return data[index];
}

template <typename T>
size_t MyVector<T>::size() const {
    return count;
}

template <typename T>
void MyVector<T>::clear() {
    delete[] data;
    data = nullptr;
    capacity = 0;
    count = 0;
    resize();
}
