#ifndef ADMIN_H
#define ADMIN_H
#include "Person.h"

class System;

class Admin : public Person
{
private:
    std::string Admin_id;

public:
    Admin(std::string Admin_id, std::string name, std::string password, std::string email, int age);
    std::string getID() const override;
    void display() const override;
};
#endif