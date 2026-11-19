#ifndef CREW_H
#define CREW_H
#include "Person.h"
#include <unordered_map>
#include <memory>
class Flight;
class Crew : public Person
{
private:
    static int max_flight_hours;
    std::string crew_id;
    bool status;
    long long int flight_hours;
    std::unordered_map<std::string, std::shared_ptr<Flight>> trips;

public:
    Crew(std::string crew_id, std::string name, std::string password, std::string email, int age);
    std::string getID() const override;
    bool getStatus() const;
    long long int getFlightHours() const;
    void setStatus(bool new_status);
    void setFlightHours(long long int new_flight_hours);
    void display() const override;
    bool canAddFlight(const std::shared_ptr<Flight> &flight, long long int flight_duration_hours) const;
    bool addFlight(std::shared_ptr<Flight> flight, long long int flight_duration_hours);
    bool removeFlight(const std::shared_ptr<Flight> &flight, long long int flight_duration_hours);
    const std::unordered_map<std::string, std::shared_ptr<Flight>> &getTrips() const;
};
#endif