#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>

using std::cout;
using std::endl;

class User {
    char* name = nullptr;
    char* egn = nullptr;
    char* password = nullptr;

    void free();
    void copyFrom(const User& other);
    void moveFrom(User&& other) noexcept;

protected:
    void setName(const char* name);
    void setEGN(const char* egn);
    void setPassword(const char* password);

public:
    User() = default;
    User(const char* name, const char* egn, const char* password);
    User(const User& other);
    User(User&& other) noexcept;

    User& operator=(const User& other);
    User& operator=(User&& other) noexcept;

    const char* getName() const;
    const char* getEGN() const;
    const char* getPassword() const;

    virtual void logout() const;
    virtual void help() const;
    virtual void viewProfile() const;

    virtual ~User();
};

