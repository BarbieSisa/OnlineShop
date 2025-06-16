#ifndef MYSTRING_H
#define MYSTRING_H

#include <iostream>

class MyString {
private:
    char* data;
    size_t length;

    void copyFrom(const char* src);
    void free();

public:
    MyString();
    MyString(const char* str);
    MyString(const MyString& other);
    MyString& operator=(const MyString& other);
    ~MyString();

    const char* c_str() const;
    size_t size() const;

    bool operator==(const MyString& other) const;
    friend std::ostream& operator<<(std::ostream& out, const MyString& str);
};

#endif
