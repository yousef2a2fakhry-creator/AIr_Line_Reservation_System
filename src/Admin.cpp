#include "../inc/Admin.h"
#include "../inc/System.h"
#include <iostream>
using namespace std;
Admin::Admin(std::string Admin_id, std::string name, std::string password, std::string email, int age) : Person(name, password, email, age), Admin_id(Admin_id) {}
std::string Admin::getID() const
{
    return Admin_id;
}

void Admin::display() const
{
    cout << "====================================\n"
         << "            Admin INFO              \n"
         << "====================================\n"
         << " ID       : " << Admin_id << "\n"
         << " Name     : " << name << "\n"
         << " Email    : " << email << "\n"
         << " Age      : " << age << "\n"
         << "====================================\n\n";
}
