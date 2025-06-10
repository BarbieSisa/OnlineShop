#include "User.h"

void User::free()
{
    delete[] name;
    delete[] egn;
    delete[] password;
}

void User::copyFrom(const User& other)
{
    int nameLen = strlen(other.name);
    name = new char[nameLen + 1];
    strcpy(name, other.name);

    int egnLen = strlen(other.egn);
    egn = new char[egnLen + 1];
    strcpy(egn, other.egn);

    int passLen = strlen(other.password);
    password = new char[passLen + 1];
    strcpy(password, other.password);
}

void User::moveFrom(User&& other) noexcept
{
    name = other.name;
    other.name = nullptr;

    egn = other.egn;
    other.egn = nullptr;

    password = other.password;
    other.password = nullptr;
}

void User::setName(const char* name)
{
    if (!name || this->name == name) {
        return;
    }

    delete[] this->name;
    int len = strlen(name);
    this->name = new char[len + 1];
    strcpy(this->name, name);
}

void User::setEGN(const char* egn)
{
    if (!egn || this->egn == egn) {
        return;
    }

    delete[] this->egn;
    int len = strlen(egn);
    this->egn = new char[len + 1];
    strcpy(this->egn, egn);
}

void User::setPassword(const char* password)
{
    if (!password || this->password == password) {
        return;
    }

    delete[] this->password;
    int len = strlen(password);
    this->password = new char[len + 1];
    strcpy(this->password, password);
}

User::User(const char* name, const char* egn, const char* password)
{
    setName(name);
    setEGN(egn);
    setPassword(password);
}

User::User(const User& other)
{
    copyFrom(other);
}

User::User(User&& other) noexcept
{
    moveFrom(std::move(other));
}

User& User::operator=(const User& other)
{
    if (this != &other) {
        free();
        copyFrom(other);
    }
    return *this;
}

User& User::operator=(User&& other) noexcept
{
    if (this != &other) {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

const char* User::getName() const
{
    return name;
}

const char* User::getEGN() const
{
    return egn;
}

const char* User::getPassword() const
{
    return password;
}

void User::logout() const
{
    cout << "Logging out user: " << (name ? name : "unknown") << endl;
}

void User::help() const
{
    cout << "Help: To change settings, update profile, or contact support.\n";
}

void User::viewProfile() const
{
    cout << "User Profile\n";
    cout << "Name: " << (name ? name : "N/A") << endl;
    cout << "EGN: " << (egn ? egn : "N/A") << endl;
    cout << "Password: " << (password ? password : "N/A") << endl;
}

User::~User()
{
    free();
}
