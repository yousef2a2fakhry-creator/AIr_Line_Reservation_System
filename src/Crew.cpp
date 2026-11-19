#include "../inc/Crew.h"
#include <iostream>
#include "../inc/Flight.h"
using namespace std;
int Crew::max_flight_hours = 40;
Crew::Crew(std::string crew_id, std::string name, std::string password, std::string email, int age) : Person(name, password, email, age), crew_id(crew_id), status(true), flight_hours(0) {}
std::string Crew::getID() const
{
    return crew_id;
}

void Crew::display() const
{
    std::cout << "====================================\n"
              << "             CREW INFO              \n"
              << "====================================\n"
              << " Name     : " << name << "\n"
              << " Email    : " << email << "\n"
              << " Age      : " << age << "\n"
              << " Crew ID  : " << crew_id << "\n"
              << " Status   : " << (status ? "Active" : "Inactive") << "\n"
              << " Flight Hours  : " << flight_hours << "\n"
              << "====================================\n\n";
}
bool Crew::getStatus() const
{
    return status;
}
void Crew::setStatus(bool new_status)
{
    status = new_status;
}
long long int Crew::getFlightHours() const
{
    return flight_hours;
}
void Crew::setFlightHours(long long int new_flight_hours)
{
    flight_hours = new_flight_hours;
}
bool Crew::canAddFlight(const shared_ptr<Flight> &ptr, long long int flight_duration_hours) const
{
    if (!ptr || flight_duration_hours <= 0 ||
        flight_hours + flight_duration_hours > max_flight_hours ||
        trips.find(ptr->getFlightNumber()) != trips.end())
        return false;

    return true;
}
bool Crew::addFlight(shared_ptr<Flight> ptr, long long int flight_duration_hours)
{
    if (!canAddFlight(ptr, flight_duration_hours))
        return false;

    trips.emplace(ptr->getFlightNumber(), ptr);
    flight_hours += flight_duration_hours;
    return true;
}
bool Crew::removeFlight(const shared_ptr<Flight> &ptr, long long int flight_duration_hours)
{
    if (!ptr || flight_duration_hours <= 0 || flight_duration_hours > flight_hours)
        return false;

    auto trip_iterator = trips.find(ptr->getFlightNumber());
    if (trip_iterator == trips.end() || trip_iterator->second != ptr)
        return false;

    trips.erase(trip_iterator);
    flight_hours -= flight_duration_hours;
    return true;
}
const unordered_map<string, shared_ptr<Flight>> &Crew::getTrips() const
{
    return trips;
}