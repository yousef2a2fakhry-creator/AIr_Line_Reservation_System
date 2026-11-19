#include "../inc/Passenger.h"
#include <iostream>
using namespace std;
Passenger::Passenger(string Passenger_id, string name, string password, string email, int age, long long int Credit, int loyalty_points) : Person(name, password, email, age), Passenger_id(Passenger_id), Credit(Credit), loyalty_points(loyalty_points < 0 ? 0 : loyalty_points) {}
std::string Passenger::getID() const
{
    return Passenger_id;
}

void Passenger::display() const
{
    cout << "====================================\n"
         << "          PASSENGER INFO            \n"
         << "====================================\n"
         << " ID       : " << Passenger_id << "\n"
         << " Name     : " << name << "\n"
         << " Email    : " << email << "\n"
         << " Age      : " << age << "\n"
         << " Credit      : " << Credit << "\n"
         << " Loyalty Points : " << loyalty_points << "\n"
         << "====================================\n\n";
}
long long int &Passenger::getCredit()
{
    return Credit;
}
void Passenger::setCredit(long long int credit)
{
    Credit = credit;
}
int Passenger::getLoyaltyPoints() const
{
    return loyalty_points;
}
void Passenger::setLoyaltyPoints(int points)
{
    loyalty_points = points < 0 ? 0 : points;
}
const string &Passenger::getPassengerId() const
{
    return Passenger_id;
}
