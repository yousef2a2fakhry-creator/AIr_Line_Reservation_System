#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

#include <string>

struct UserData
{
    std::string id;
    std::string name;
    std::string password;
    int age;
    std::string email;
};
struct CrewData : public UserData
{
    long long int flight_hours = 0;
};
struct PassengerData : public UserData
{
    long long int credit = 0;
    int points = 0;
};
struct AircraftData
{
    std::string aircraft_id;
    std::string type;
    int capacity;
    bool status;
};

struct FlightData
{
    std::string flight_number;
    std::string origin;
    std::string destination;
    std::string departure_time;
    std::string arrival_time;
    std::string aircraft_id;
    std::string status;
    double price = 0.0;
};

struct ReservationData
{
    std::string reservation_id;
    std::string passenger_id;
    std::string flight_number;
    std::string seat_id;
    std::string payment_id;
    std::string status;
};

#endif