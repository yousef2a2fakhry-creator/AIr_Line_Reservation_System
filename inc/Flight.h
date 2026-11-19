#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>
#include <memory>
#include <unordered_map>

class Passenger;
class AirCraft;

class Flight
{
private:
    std::string flight_number;
    std::string origin;
    std::string destination;
    std::string departure_time;
    std::string arrival_time;
    std::string status;
    double price;
    std::shared_ptr<AirCraft> aircraft;
    std::unordered_map<std::string, std::shared_ptr<Passenger>> passengers;

public:
    Flight(std::string flight_number,
           std::string origin,
           std::string destination,
           std::string departure_time,
           std::string arrival_time,
           std::shared_ptr<AirCraft> aircraft,
           std::string status = "Scheduled",
           double price = 0.0);

    const std::string &getFlightNumber() const;
    const std::string &getOrigin() const;
    const std::string &getDestination() const;
    const std::string &getDepartureTime() const;
    const std::string &getArrivalTime() const;
    const std::string &getStatus() const;
    double getPrice() const;
    void setStatus(const std::string &status);
    void setPrice(double price);
    void display() const;

    std::shared_ptr<AirCraft> getAircraft() const;
    bool addPassenger(const std::shared_ptr<Passenger> &passenger);
    bool deletePassenger(const std::string &passenger_id);
    std::shared_ptr<Passenger> findPassenger(const std::string &passenger_id) const;
    const std::unordered_map<std::string, std::shared_ptr<Passenger>> &getPassengers() const;
};

#endif