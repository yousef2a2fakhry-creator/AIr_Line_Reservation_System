#ifndef SYSTEM_H
#define SYSTEM_H
#include <unordered_map>
#include <optional>
#include <memory>
#include <vector>
#include <ctime>
#include "Person.h"
#include <mutex>

class Reservation;
class Flight;
class AirCraft;

struct Udata
{
    std::string id;
    bool requires_password_change;
};
class System
{
private:
    std::mutex reservation_mutex;
    std::unordered_map<std::string, std::shared_ptr<Person>> users;
    std::unordered_map<std::string, std::shared_ptr<Flight>> flights;
    std::unordered_map<std::string, std::shared_ptr<AirCraft>> fleet;
    std::unordered_map<std::string, std::shared_ptr<Reservation>> reservations;

    std::string generate_uuid();
    bool isAdmin(const std::string &id);
    bool isPasswordDefaulted(const std::string &tested_password);
    bool parseCrewFlightTime(const std::string &value, std::time_t &timestamp);
    long long int getCrewFlightDurationHours(const Flight &flight);

public:
    std::optional<Udata> Login(int login_choice, std::string &enterd_name, std::string &password);
    bool createStaffUser(const std::string &current_admin_id, const std::string &role, std::string name, std::string password, std::string email, int age);
    bool registerUser(std::string name, std::string password, std::string email, int age);
    bool addPassengerCredit(const std::string &passenger_id, long long int amount);

    void removeUser(const std::string &admin_id, const std::string &id);
    bool addAircraft(const std::string &admin_id, std::string aircraft_id, std::string type, size_t capacity, bool status = true);
    bool scheduleFlight(const std::string &admin_id, std::string flight_number, std::string origin, std::string destination, std::string dep_time, std::string arr_time, std::string aircraft_id, double price);

    void UpdateUserPassword(int login_choice, const std::string &userId, const std::string &new_password);
    std::shared_ptr<Person> getUser(std::string id);

    bool makeReservation(const std::string &passenger_id, const std::string &flight_number, const std::string &seat_id);
    std::vector<std::shared_ptr<Flight>> getCrewFlights(const std::string &crew_id) const;
    std::vector<std::shared_ptr<Reservation>> getReservationsForPassenger(const std::string &passenger_id) const;
    bool cancelReservation(const std::string &requester_id, const std::string &reservation_id);

    std::string getCurrentDate() const;
    bool loadData();

    std::vector<std::shared_ptr<Flight>> searchFlights(const std::string &origin = "", const std::string &destination = "", double max_price = -1.0, const std::string &dep_date = "");
    void sortByPrice(std::vector<std::shared_ptr<Flight>> &flight_list, bool ascending = true);
    void sortByDepartureTime(std::vector<std::shared_ptr<Flight>> &flight_list);
    bool assignCrewMemberToFlight(const std::string &id, const std::string &f_id);

    bool showSeats(std::string flight_id);
};
#endif