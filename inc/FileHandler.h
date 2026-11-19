#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <fstream>
#include <string>
#include <optional>
#include <vector>
#include "DataStructs.h"

struct UserInfo
{
    std::string id;
    std::string password;
};
class FileHandler
{
private:
    std::fstream file;
    std::string file_name;
    bool IS_User_Name_Present(const std::string &name);

public:
    FileHandler(const std::string &file_name);
    void Change_Password(const std::string &target_id, const std::string &new_password);

    // function used in the login to check the hashed passwords
    std::optional<UserInfo> Get_Hashed_Password(const std::string &target_username);

    bool Save_User(const std::string &name, const std::string &password, const std::string &email, int age, const std::string &id);
    bool Save_Crew(const std::string &name, const std::string &password, const std::string &email, int age, const std::string &id, long long int flight_hours = 0);
    bool Save_Passenger(const std::string &name, const std::string &password, const std::string &email, int age, const std::string &id, long long int credit, int points = 0);
    bool Save_Aircraft(const std::string &aircraft_id, const std::string &type, int capacity, bool status = true);
    bool Save_Flight(const std::string &flight_number, const std::string &origin, const std::string &destination, const std::string &departure_time, const std::string &arrival_time, const std::string &aircraft_id, const std::string &status, double price);
    bool Save_Reservation(const std::string &reservation_id, const std::string &passenger_id, const std::string &flight_number, const std::string &seat_id, const std::string &payment_id, const std::string &status);
    bool Save_CrewFlight(const std::string &crew_id, const std::string &flight_id);
    bool Update_Reservation_Status(const std::string &reservation_id, const std::string &status);
    bool Update_Aircraft_status(const std::string &aircraft_id, bool status);
    bool Update_Passenger_Credit(const std::string &passenger_id, long long int new_credit);
    bool Update_Points(const std::string &passenger_id, int new_points);
    bool Update_Crew_Flight_Hours(const std::string &crew_id, long long int flight_hours);

    /*
    delete a record in any file based in the id
    */
    bool Delete_Record(const std::string &target_id, int id_column_index = 0);

    /*
    functions for reading the data form the CSV files when starting the system
    */
    std::vector<UserData> Load_Users();
    std::vector<CrewData> Load_Crew();
    std::vector<PassengerData> Load_Passengers();
    std::vector<AircraftData> Load_Aircraft();
    std::vector<FlightData> Load_Flights();
    std::vector<ReservationData> Load_Reservations();

    ~FileHandler();
};

#endif