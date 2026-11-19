#include <iostream>
#include "../inc/System.h"
#include "../inc/FileHandler.h"
#include "../inc/Passenger.h"
#include "../inc/Admin.h"
#include "../inc/BookingAgent.h"
#include "../inc/Crew.h"
#include "../inc/sha256.h"
#include "../inc/AirCraft.h"
#include "../inc/Flight.h"
#include "../inc/Reservation.h"
#include "../inc/Payment.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>
using namespace std;

/*
    functions for extraction the hours of flight form the departure and the arrival time
*/
bool System::parseCrewFlightTime(const string &value, time_t &timestamp)
{
    tm parsed_time = {};
    istringstream input(value);
    input >> get_time(&parsed_time, "%Y-%m-%d");
    if (input.fail())
        return false;

    input >> ws;
    if (!input.eof())
    {
        input >> get_time(&parsed_time, "%H:%M");
        if (input.fail())
            return false;
        input >> ws;
        if (!input.eof())
            return false;
    }

    int expected_year = parsed_time.tm_year;
    int expected_month = parsed_time.tm_mon;
    int expected_day = parsed_time.tm_mday;
    int expected_hour = parsed_time.tm_hour;
    int expected_minute = parsed_time.tm_min;
    parsed_time.tm_isdst = -1;
    time_t converted = mktime(&parsed_time);
    if (converted == static_cast<time_t>(-1) ||
        parsed_time.tm_year != expected_year || parsed_time.tm_mon != expected_month ||
        parsed_time.tm_mday != expected_day || parsed_time.tm_hour != expected_hour ||
        parsed_time.tm_min != expected_minute)
        return false;

    timestamp = converted;
    return true;
}
long long int System::getCrewFlightDurationHours(const Flight &flight)
{
    time_t departure = 0;
    time_t arrival = 0;
    if (!parseCrewFlightTime(flight.getDepartureTime(), departure) ||
        !parseCrewFlightTime(flight.getArrivalTime(), arrival))
        return -1;

    double duration_seconds = difftime(arrival, departure);
    if (duration_seconds <= 0)
        return -1;
    return static_cast<long long int>(ceil(duration_seconds / 3600.0));
}
string System::getCurrentDate() const
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char dateBuffer[20];
    strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", ltm);
    return string(dateBuffer);
}

/*
    genetrate 32 hexadecimal digits representing the id
*/
string System::generate_uuid()
{
    auto id = boost::uuids::random_generator()();
    return boost::uuids::to_string(id);
}
/*
    checking for the admin actions first
*/
bool System::isAdmin(const string &id)
{
    auto it = users.find(id);
    if (it != users.end())
    {
        return dynamic_pointer_cast<Admin>(it->second) != nullptr;
    }
    return false;
}

bool System::registerUser(string name, string password, string email, int age)
{
    string uuid_str = generate_uuid();
    FileHandler file("data/Passengers.csv");
    SHA256 sha256;
    password = sha256(password);

    if (!file.Save_Passenger(name, password, email, age, uuid_str, 0, 0))
    {
        return false;
    }

    shared_ptr<Person> ptr = make_shared<Passenger>(uuid_str, name, password, email, age, 0, 0);
    users[uuid_str] = ptr;
    return true;
}

bool System::addPassengerCredit(const string &passenger_id, long long int amount)
{
    if (amount <= 0)
    {
        return false;
    }

    auto user_iterator = users.find(passenger_id);
    if (user_iterator == users.end())
    {
        return false;
    }

    auto passenger = dynamic_pointer_cast<Passenger>(user_iterator->second);
    if (!passenger)
    {
        return false;
    }

    long long int new_credit = passenger->getCredit() + amount;
    FileHandler passenger_file("data/Passengers.csv");
    if (!passenger_file.Update_Passenger_Credit(passenger_id, new_credit))
    {
        return false;
    }

    passenger->setCredit(new_credit);
    return true;
}

bool System::createStaffUser(const string &current_admin_id, const string &role, string name, string password, string email, int age)
{
    if (!isAdmin(current_admin_id))
    {
        return false;
    }

    string file_name;
    if (role == "Admin")
    {
        file_name = "data/Admin.csv";
    }
    else if (role == "BookingAgent")
    {
        file_name = "data/BookingAgent.csv";
    }
    else if (role == "Crew")
    {
        file_name = "data/Crew.csv";
    }
    else
    {
        file_name = "data/users.csv";
    }
    string user_id = generate_uuid();
    password = SHA256()(password);
    FileHandler handler(file_name);
    bool saved = role == "Crew" ? handler.Save_Crew(name, password, email, age, user_id) : handler.Save_User(name, password, email, age, user_id);
    if (!saved)
    {
        return false;
    }

    shared_ptr<Person> new_user = nullptr;

    if (role == "Admin")
    {
        new_user = make_shared<Admin>(user_id, name, password, email, age);
    }
    else if (role == "BookingAgent")
    {
        new_user = make_shared<BookingAgent>(user_id, name, password, email, age);
    }
    else if (role == "Crew")
    {
        new_user = make_shared<Crew>(user_id, name, password, email, age);
    }

    if (new_user)
    {
        users[user_id] = new_user;
        return true;
    }

    return false;
}
optional<Udata> System::Login(int login_choice, string &enterd_name, string &password)
{
    string filename;
    if (login_choice == 1)
    {
        filename = "data/Passengers.csv";
    }
    else if (login_choice == 2)
    {
        filename = "data/BookingAgent.csv";
    }
    else if (login_choice == 3)
    {
        filename = "data/Admin.csv";
    }
    else if (login_choice == 4)
    {
        filename = "data/Crew.csv";
    }
    else
    {
        cerr << "Invalid choice for login" << endl;
        return nullopt;
    }

    FileHandler file(filename);
    optional<UserInfo> pass = file.Get_Hashed_Password(enterd_name);

    SHA256 sha256;
    string tested_password = sha256(password);

    if (pass.has_value() && pass->password == tested_password)
    {
        Udata state;
        state.id = pass->id;
        state.requires_password_change = isPasswordDefaulted(password);
        return state;
    }
    return nullopt;
}

bool System::addAircraft(const string &admin_id, string aircraft_id, string type, size_t capacity, bool status)
{
    if (!isAdmin(admin_id))
        return false;

    if (fleet.find(aircraft_id) != fleet.end())
        return false;

    FileHandler aircraftFile("data/aircraft.csv");
    if (!aircraftFile.Save_Aircraft(aircraft_id, type, capacity, status))
    {
        return false;
    }

    auto new_aircraft = make_shared<AirCraft>(aircraft_id, type, capacity, status);
    fleet[aircraft_id] = new_aircraft;
    new_aircraft->generateSeats();
    return true;
}

bool System::scheduleFlight(const string &admin_id, string flight_number, string origin, string destination, string dep_time, string arr_time, string aircraft_id, double price)
{
    if (!isAdmin(admin_id))
        return false;

    if (flights.find(flight_number) != flights.end())
        return false;

    if (fleet.find(aircraft_id) == fleet.end())
        return false;

    FileHandler flightFile("data/Flight.csv");
    if (!flightFile.Save_Flight(flight_number, origin, destination, dep_time, arr_time, aircraft_id, "Scheduled", price))
    {
        return false;
    }

    auto new_flight = make_shared<Flight>(flight_number, origin, destination, dep_time, arr_time, fleet[aircraft_id], "Scheduled", price);
    flights[flight_number] = new_flight;

    return true;
}

void System::UpdateUserPassword(int login_choice, const string &userId, const string &new_password)
{
    string filename;
    if (login_choice == 1)
    {
        filename = "data/Passengers.csv";
    }
    else if (login_choice == 2)
    {
        filename = "data/BookingAgent.csv";
    }
    else if (login_choice == 3)
    {
        filename = "data/Admin.csv";
    }
    else if (login_choice == 4)
    {
        filename = "data/Crew.csv";
    }
    else
    {
        cerr << "Error: Invalid login choice. Cannot update password.\n";
        return;
    }
    SHA256 sha256;
    string hashed_password = sha256(new_password);
    auto it = users.find(userId);
    if (it != users.end() && it->second != nullptr)
    {
        it->second->setPassword(hashed_password);
    }
    FileHandler file(filename);
    file.Change_Password(userId, hashed_password);
}

/*
    this is the first functions executed in the runtime for loading the system as what it was
    in the previous state
*/
bool System::loadData()
{
    FileHandler aircraftFile("data/AirCraft.csv");
    vector<AircraftData> loaded_aircraft = aircraftFile.Load_Aircraft();
    for (const auto &a : loaded_aircraft)
    {
        fleet[a.aircraft_id] = make_shared<AirCraft>(a.aircraft_id, a.type, a.capacity, a.status);
        fleet[a.aircraft_id]->generateSeats();
    }
    FileHandler passengerFile("data/Passengers.csv");
    vector<PassengerData> loaded_passengers = passengerFile.Load_Passengers();
    for (const auto &p : loaded_passengers)
    {
        auto passenger = make_shared<Passenger>(p.id, p.name, p.password, p.email, p.age, p.credit, p.points);
        users[p.id] = passenger;
    }
    FileHandler adminFile("data/Admin.csv");
    vector<UserData> loaded_admins = adminFile.Load_Users();
    for (const auto &adm : loaded_admins)
    {
        auto admin = make_shared<Admin>(adm.id, adm.name, adm.password, adm.email, adm.age);
        users[adm.id] = admin;
    }
    FileHandler agentFile("data/BookingAgent.csv");
    vector<UserData> loaded_agents = agentFile.Load_Users();
    for (const auto &ag : loaded_agents)
    {
        auto agent = make_shared<BookingAgent>(ag.id, ag.name, ag.password, ag.email, ag.age);
        users[ag.id] = agent;
    }
    FileHandler crewFile("data/Crew.csv");
    vector<CrewData> loaded_crew = crewFile.Load_Crew();
    for (const auto &c : loaded_crew)
    {
        auto crew_member = make_shared<Crew>(c.id, c.name, c.password, c.email, c.age);
        crew_member->setFlightHours(c.flight_hours);
        users[c.id] = crew_member;
    }
    FileHandler flightFile("data/Flight.csv");
    vector<FlightData> loaded_flights = flightFile.Load_Flights();
    for (const auto &f : loaded_flights)
    {
        auto aircraft_it = fleet.find(f.aircraft_id);
        if (aircraft_it != fleet.end())
        {
            flights[f.flight_number] = make_shared<Flight>(
                f.flight_number,
                f.origin,
                f.destination,
                f.departure_time,
                f.arrival_time,
                aircraft_it->second,
                f.status,
                f.price);
        }
    }
    FileHandler reservationFile("data/Reservation.csv");
    vector<ReservationData> loaded_reservations = reservationFile.Load_Reservations();
    for (const auto &r : loaded_reservations)
    {
        auto user_it = users.find(r.passenger_id);
        auto flight_it = flights.find(r.flight_number);
        if (user_it != users.end() && flight_it != flights.end())
        {
            auto passenger = dynamic_pointer_cast<Passenger>(user_it->second);
            auto flight = flight_it->second;
            if (passenger && flight)
            {
                auto payment = make_shared<Payment>(r.payment_id, 0.0, "Restored");
                auto reservation = make_shared<Reservation>(
                    r.reservation_id,
                    passenger,
                    flight,
                    r.seat_id,
                    payment,
                    r.status);
                if (r.status == "Confirmed" && flight->getAircraft() != nullptr)
                {
                    flight->getAircraft()->reserveSeat(r.seat_id);
                    flight->addPassenger(passenger);
                }

                reservations[r.reservation_id] = reservation;
            }
        }
    }

    return true;
}
vector<shared_ptr<Flight>> System::searchFlights(const string &origin, const string &destination, double max_price, const string &dep_date)
{
    vector<shared_ptr<Flight>> filtered_flights;
    vector<shared_ptr<Flight>> all_flights;
    all_flights.reserve(flights.size());
    for (const auto &[flight_num, flight_ptr] : flights)
    {
        all_flights.push_back(flight_ptr);
    }
    auto matchesCriteria = [&](const shared_ptr<Flight> &f) -> bool
    {
        if (!f)
            return false;
        if (!origin.empty() && f->getOrigin() != origin)
        {
            return false;
        }
        if (!destination.empty() && f->getDestination() != destination)
        {
            return false;
        }
        if (max_price >= 0.0 && f->getPrice() > max_price)
        {
            return false;
        }
        if (!dep_date.empty() && f->getDepartureTime().find(dep_date) == string::npos)
        {
            return false;
        }
        return true;
    };
    std::copy_if(all_flights.begin(), all_flights.end(), back_inserter(filtered_flights), matchesCriteria);
    return filtered_flights;
}

/*
    filters for the search oprations by the price also by the departure time
*/
void System::sortByPrice(vector<shared_ptr<Flight>> &flight_list, bool ascending)
{
    sort(flight_list.begin(), flight_list.end(),
         [ascending](const shared_ptr<Flight> &a, const shared_ptr<Flight> &b)
         {
             if (ascending)
             {
                 return a->getPrice() < b->getPrice();
             }
             else
             {
                 return a->getPrice() > b->getPrice();
             }
         });
}
void System::sortByDepartureTime(vector<shared_ptr<Flight>> &flight_list)
{
    sort(flight_list.begin(), flight_list.end(),
         [](const shared_ptr<Flight> &a, const shared_ptr<Flight> &b)
         {
             return a->getDepartureTime() < b->getDepartureTime();
         });
}

bool System::makeReservation(const string &passenger_id, const string &flight_number, const string &seat_id)
{
    /*
    purpose : prevent the race condiditon for a specific seat inside the airplane
    i have no unlocked it as it will be automatically unlocked after the end of the function
    */
    std::lock_guard<std::mutex> lock(reservation_mutex);

    if (flights.find(flight_number) == flights.end())
    {
        return false;
    }
    if (users.find(passenger_id) == users.end())
    {
        return false;
    }
    auto passenger = dynamic_pointer_cast<Passenger>(users[passenger_id]);
    if (!passenger)
    {
        return false;
    }
    auto flight = flights[flight_number];
    constexpr int points_threshold = 5000;
    constexpr double redemption_discount = 50.0;
    int earned_points = 0;
    if (flight->getPrice() > 0.0)
    {
        double fare_points = min(floor(flight->getPrice()), static_cast<double>(numeric_limits<int>::max()));
        earned_points = static_cast<int>(fare_points);
    }
    long long int pending_points = static_cast<long long int>(passenger->getLoyaltyPoints()) + earned_points;
    bool redeem_discount = pending_points >= points_threshold;
    int updated_points = redeem_discount ? 0 : static_cast<int>(pending_points);
    double discount = redeem_discount ? min(redemption_discount, flight->getPrice() * 0.5) : 0.0;

    string res_id = generate_uuid();
    string pay_id = generate_uuid();

    auto payment = make_shared<Payment>(pay_id, flight->getPrice() - discount, getCurrentDate());
    auto reservation = make_shared<Reservation>(res_id, passenger, flight, seat_id, payment);

    double user_balance = passenger->getCredit();

    if (!reservation->confirmReservation(user_balance))
    {
        return false;
    }

    FileHandler reservationFile("data/Reservation.csv");
    if (!reservationFile.Save_Reservation(res_id, passenger_id, flight_number, seat_id, pay_id, "Confirmed"))
    {
        reservation->cancelReservation(user_balance);
        return false;
    }

    FileHandler passengerFile("data/Passengers.csv");
    int previous_points = passenger->getLoyaltyPoints();
    if (!passengerFile.Update_Points(passenger_id, updated_points))
    {
        reservation->cancelReservation(user_balance);
        reservationFile.Update_Reservation_Status(res_id, "Canceled");
        return false;
    }
    if (!passengerFile.Update_Passenger_Credit(passenger_id, static_cast<long long int>(user_balance)))
    {
        passengerFile.Update_Points(passenger_id, previous_points);
        reservation->cancelReservation(user_balance);
        reservationFile.Update_Reservation_Status(res_id, "Canceled");
        return false;
    }

    passenger->setCredit(static_cast<long long int>(user_balance));
    passenger->setLoyaltyPoints(updated_points);
    reservations[res_id] = reservation;

    return true;
}

vector<shared_ptr<Flight>> System::getCrewFlights(const string &crew_id) const
{
    auto user_iterator = users.find(crew_id);
    if (user_iterator == users.end())
        return {};

    auto crew_member = dynamic_pointer_cast<Crew>(user_iterator->second);
    if (!crew_member)
        return {};

    vector<shared_ptr<Flight>> assigned_flights;
    const auto &trips = crew_member->getTrips();
    assigned_flights.reserve(trips.size());
    for (const auto &[flight_number, flight] : trips)
    {
        if (flight)
            assigned_flights.push_back(flight);
    }
    return assigned_flights;
}

vector<shared_ptr<Reservation>> System::getReservationsForPassenger(const string &passenger_id) const
{
    vector<shared_ptr<Reservation>> passenger_reservations;
    for (const auto &[reservation_id, reservation] : reservations)
    {
        if (reservation && reservation->getPassenger() &&
            reservation->getPassenger()->getPassengerId() == passenger_id)
        {
            passenger_reservations.push_back(reservation);
        }
    }
    return passenger_reservations;
}

bool System::cancelReservation(const string &requester_id, const string &reservation_id)
{
    auto reservation_iterator = reservations.find(reservation_id);
    if (reservation_iterator == reservations.end() || !reservation_iterator->second)
        return false;

    auto requester_iterator = users.find(requester_id);
    if (requester_iterator == users.end() || !requester_iterator->second)
        return false;

    auto reservation = reservation_iterator->second;
    bool is_owner = reservation->getPassenger() &&
                    reservation->getPassenger()->getPassengerId() == requester_id;
    bool is_agent = dynamic_pointer_cast<BookingAgent>(requester_iterator->second) != nullptr;
    if ((!is_owner && !is_agent) || !reservation->getPassenger())
        return false;

    double user_credit = reservation->getPassenger()->getCredit();
    if (!reservation->cancelReservation(user_credit))
        return false;

    reservation->getPassenger()->setCredit(static_cast<long long int>(user_credit));
    FileHandler reservation_file("data/Reservation.csv");
    return reservation_file.Update_Reservation_Status(reservation_id, "Canceled");
}

bool System::assignCrewMemberToFlight(const string &id, const string &f_id)
{
    auto user_iterator = users.find(id);
    auto flight_iterator = flights.find(f_id);
    if (user_iterator == users.end() || flight_iterator == flights.end())
    {
        return false;
    }

    auto crew_member = dynamic_pointer_cast<Crew>(user_iterator->second);
    auto flight = flight_iterator->second;

    if (!crew_member)
    {
        return false;
    }
    long long int flight_duration_hours = getCrewFlightDurationHours(*flight);
    if (!crew_member->canAddFlight(flight, flight_duration_hours))
    {
        return false;
    }
    long long int original_flight_hours = crew_member->getFlightHours();
    long long int updated_flight_hours = original_flight_hours + flight_duration_hours;

    if (!crew_member->addFlight(flight, flight_duration_hours))
    {
        return false;
    }

    FileHandler crew_file("data/Crew.csv");
    if (!crew_file.Update_Crew_Flight_Hours(id, updated_flight_hours))
    {
        crew_member->removeFlight(flight, flight_duration_hours);
        return false;
    }

    FileHandler crew_flight_file("data/CrewFlight.csv");
    if (!crew_flight_file.Save_CrewFlight(id, f_id))
    {
        crew_member->removeFlight(flight, flight_duration_hours);
        crew_file.Update_Crew_Flight_Hours(id, original_flight_hours);
        return false;
    }
    return true;
}
bool System::showSeats(std::string flight_id)
{
    if (!flights[flight_id])
    {
        return false;
    }
    else
    {
        flights[flight_id]->getAircraft()->displaySeatMap();
    }
    return true;
}
void System::removeUser(const string &admin_id, const string &id)
{
    if (!isAdmin(admin_id))
    {
        return;
    }
    users.erase(id);
}
shared_ptr<Person> System::getUser(string id)
{
    auto user_iterator = users.find(id);
    if (user_iterator == users.end())
        return nullptr;
    return user_iterator->second;
}
bool System::isPasswordDefaulted(const string &tested_password)
{
    return tested_password == "Admin";
}
