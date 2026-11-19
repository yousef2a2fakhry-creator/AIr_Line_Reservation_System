#include "../inc/Person.h"
#include "../inc/sha256.h"
#include "../inc/FileHandler.h"
#include <iostream>
using namespace std;

Person::Person(string name, string password, string email, int age): name(name),password(SHA256()(password)),email(email),age(age){}


void Person::setName(const std::string &name)
{
    this->name = name;
}

void Person::setPassword(const std::string &password)
{
    this->password = password;
}

void Person::setEmail(const std::string &email)
{
    this->email = email;
}

std::string Person::getEmail() const
{
    return email;
}

void Person::setAge(int age)
{
    this->age = age;
}

std::string Person::getName() const
{
    return name;
}

std::string Person::getPassword() const
{
    return password;
}

int Person::getAge() const
{
    return age;
}