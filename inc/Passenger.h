#ifndef PASSENGER_H
#define PASSENGER_H
#include <vector>
#include "Person.h"
#include <unordered_map>
#include "Flight.h"

class Passenger : public Person
{
private:
    std::string Passenger_id;
    long long int Credit;
    int loyalty_points;

public:
    Passenger(std::string Passenger_id, std::string name, std::string password, std::string email, int age, long long int Credit, int loyalty_points = 0);
    std::string getID() const override;
    long long int &getCredit();
    void setCredit(long long int credit);
    int getLoyaltyPoints() const;
    void setLoyaltyPoints(int points);
    const std::string &getPassengerId() const;
    virtual void display() const override;
};

#endif