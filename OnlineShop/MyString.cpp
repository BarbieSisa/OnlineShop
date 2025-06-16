#include "MyString.h"
#include <cstring>

void MyString::copyFrom(const char* src) {
    length = strlen(src);
    data = new char[length + 1];
    strcpy(data, src);
}
void MyString::free() {
    delete[] data;
}
MyString::MyString() : data(nullptr), length(0) {
    data = new char[1];
    data[0] = '\0';
}
MyString::MyString(const char* str) {
    copyFrom(str);
}
MyString::MyString(const MyString& other) {
    copyFrom(other.data);
}
MyString& MyString::operator=(const MyString& other) {
    if (this != &other) {
        free();
        copyFrom(other.data);
    }
    return *this;
}
MyString::~MyString() {
    free();
}
const char* MyString::c_str() const {
    return data;
}
size_t MyString::size() const {
    return length;
}
bool MyString::operator==(const MyString& other) const {
    return strcmp(data, other.data) == 0;
}

std::ostream& operator<<(std::ostream& out, const MyString& str) {
    out << str.data;
    return out;
}
