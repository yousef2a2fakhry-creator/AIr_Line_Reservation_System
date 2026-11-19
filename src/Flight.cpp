#include "../inc/Flight.h"
#include "../inc/AirCraft.h"
#include "../inc/Passenger.h"
#include <iomanip>
#include <iostream>
#include <utility>

using namespace std;
Flight::Flight(std::string flight_number,
               std::string origin,
               std::string destination,
               std::string departure_time,
               std::string arrival_time,
               shared_ptr<AirCraft> aircraft,
               std::string status,
               double price)
    : flight_number(flight_number),
      origin(origin),
      destination(destination),
      departure_time(departure_time),
      arrival_time(arrival_time),
      status(status),
      price(price),
      aircraft(aircraft)
{
}

const string &Flight::getFlightNumber() const { return flight_number; }
const string &Flight::getOrigin() const { return origin; }
const string &Flight::getDestination() const { return destination; }
const string &Flight::getDepartureTime() const { return departure_time; }
const string &Flight::getArrivalTime() const { return arrival_time; }
const string &Flight::getStatus() const { return status; }
double Flight::getPrice() const { return price; }

void Flight::display() const
{
    cout << "\n--- FLIGHT INFORMATION ---\n";
    cout << "Flight number: " << flight_number << '\n';
    cout << "Route: " << origin << " -> " << destination << '\n';
    cout << "Departure: " << departure_time << '\n';
    cout << "Arrival: " << arrival_time << '\n';
    cout << "Status: " << status << '\n';
    cout << fixed << setprecision(2) << "Price: $" << price << '\n';

    if (aircraft)
    {
        cout << "Aircraft: " << aircraft->getId()
             << " (" << aircraft->getType() << ")\n";
        cout << "Capacity: " << aircraft->getCapacity() << '\n';
    }
    else
    {
        cout << "Aircraft: Not assigned\n";
    }
}

void Flight::setStatus(const string &new_status)
{
    status = new_status;
}

void Flight::setPrice(double new_price)
{
    price = new_price;
}

shared_ptr<AirCraft> Flight::getAircraft() const
{
    return aircraft;
}

bool Flight::addPassenger(const shared_ptr<Passenger> &passenger)
{
    if (passenger == nullptr)
    {
        return false;
    }
    return passengers.emplace(passenger->getPassengerId(), passenger).second;
}

bool Flight::deletePassenger(const string &passenger_id)
{
    return passengers.erase(passenger_id) > 0;
}

shared_ptr<Passenger> Flight::findPassenger(const string &passenger_id) const
{
    auto passenger_iterator = passengers.find(passenger_id);
    if (passenger_iterator == passengers.end())
    {
        return nullptr;
    }
    return passenger_iterator->second;
}

const unordered_map<string, shared_ptr<Passenger>> &Flight::getPassengers() const
{
    return passengers;
}
