#include "../inc/BookingAgent.h"
#include <iostream>
using namespace std;
BookingAgent::BookingAgent(std::string BookingAgent_id, std::string name, std::string password, std::string email, int age) : Person(name, password, email, age), BookingAgent_id(BookingAgent_id) {}
std::string BookingAgent::getID() const
{
    return BookingAgent_id;
}

void BookingAgent::display() const
{
    cout << "====================================\n"
         << "       Booking Agent INFO       \n"
         << "====================================\n"
         << " ID       : " << BookingAgent_id << "\n"
         << " Name     : " << name << "\n"
         << " Email    : " << email << "\n"
         << " Age      : " << age << "\n"
         << "====================================\n\n";
}