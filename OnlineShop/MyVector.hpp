#ifndef MY_VECTOR_HPP
#define MY_VECTOR_HPP

#include <stdexcept>

template <typename T>
class MyVector {
private:
    T* data;
    size_t capacity;
    size_t length;

    void resize(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < length; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }

public:
    MyVector() : data(nullptr), capacity(0), length(0) {
        resize(4);  // default initial capacity
    }

    ~MyVector() {
        delete[] data;
    }

    MyVector(const MyVector& other) {
        capacity = other.capacity;
        length = other.length;
        data = new T[capacity];
        for (size_t i = 0; i < length; ++i) {
            data[i] = other.data[i];
        }
    }

    MyVector& operator=(const MyVector& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            length = other.length;
            data = new T[capacity];
            for (size_t i = 0; i < length; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    void push_back(const T& value) {
        if (length >= capacity) {
            resize(capacity * 2);
        }
        data[length++] = value;
    }

    void pop_back() {
        if (length == 0) throw std::out_of_range("Vector is empty");
        --length;
    }
    void erase(size_t index) {
        if (index >= size) return;

        for (size_t i = index; i < size - 1; ++i) {
            data[i] = data[i + 1]; 
        }

        --size; 
    }
    T& operator[](size_t index) {
        if (index >= length) throw std::out_of_range("Index out of bounds");
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= length) throw std::out_of_range("Index out of bounds");
        return data[index];
    }

    size_t size() const {
        return length;
    }

    bool empty() const {
        return length == 0;
    }

    void clear() {
        length = 0;
    }
    T* begin() {
        return data;
    }

    T* end() {
        return data + size;
    }
};

#endif 
