#ifndef BOOKING_H
#define BOOKING_H
#include "Person.h"
class BookingAgent : public Person
{
private:
    std::string BookingAgent_id;

public:
    BookingAgent(std::string BookingAgent_id, std::string name, std::string password, std::string email, int age);
    std::string getID() const override;
    void display() const override;
};

#endif