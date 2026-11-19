#include "../inc/FileHandler.h"
#include <iostream>
#include <sstream>

using namespace std;

// Helper function to safely insert "temp_" before the filename without breaking directory paths
static string Get_Temp_File_Name(const string &file_path)
{
    size_t last_slash = file_path.find_last_of("/\\");
    if (last_slash == string::npos)
    {
        return "temp_" + file_path;
    }
    string dir = file_path.substr(0, last_slash + 1);
    string filename = file_path.substr(last_slash + 1);
    return dir + "temp_" + filename;
}

FileHandler::FileHandler(const string &file_name) : file_name(file_name)
{
}

bool FileHandler::IS_User_Name_Present(const string &name)
{
    file.open(file_name, ios::in);
    if (!file.is_open())
    {
        cerr << "Error: Could not open the file " << file_name << " for reading." << endl;
        return false;
    }
    string line;
    getline(file, line);
    while (getline(file, line))
    {
        stringstream ss(line);
        string id, stored_name, password, age;
        getline(ss, id, ',');
        getline(ss, stored_name, ',');
        getline(ss, password, ',');
        getline(ss, age, ',');

        if (!stored_name.empty() && stored_name.front() == '"')
        {
            stored_name.erase(0, 1);
        }
        if (!stored_name.empty() && stored_name.back() == '"')
        {
            stored_name.erase(stored_name.size() - 1);
        }
        if (stored_name == name)
        {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

void FileHandler::Change_Password(const string &target_id, const string &new_password)
{
    file.clear();
    file.close();

    file.open(file_name, ios::in);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file for reading: " << file_name << endl;
        return;
    }

    string temp_file_name = Get_Temp_File_Name(file_name);
    ofstream tempFile(temp_file_name);
    if (!tempFile.is_open())
    {
        cerr << "Error: Could not create temporary file." << endl;
        file.close();
        return;
    }

    string line;
    bool updated = false;

    if (getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        tempFile << line << "\n";
    }

    while (getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        stringstream ss(line);
        string id, name, password, age, email;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, password, ',');
        getline(ss, age, ',');
        getline(ss, email, ',');
        if (id == target_id)
        {
            string additional_fields;
            getline(ss, additional_fields);
            tempFile << id << "," << name << "," << new_password << "," << age << "," << email;
            if (!additional_fields.empty())
                tempFile << "," << additional_fields;
            tempFile << "\n";
            updated = true;
        }
        else
        {
            tempFile << line << "\n";
        }
    }

    file.close();
    tempFile.close();

    if (!updated)
    {
        remove(temp_file_name.c_str());
        return;
    }

    remove(file_name.c_str());
    rename(temp_file_name.c_str(), file_name.c_str());
}

bool FileHandler::Save_User(const string &name, const string &password, const string &email, int age, const string &id)
{
    if (IS_User_Name_Present(name))
    {
        return false;
    }

    file.open(file_name, ios::app);
    if (!file.is_open())
    {
        cerr << "Error: Could not open the file " << file_name << " for writing." << endl;
        return false;
    }
    file << id << "," << name << "," << password << "," << age << "," << email << "\n";
    file.close();
    return true;
}

bool FileHandler::Save_Crew(const string &name, const string &password, const string &email, int age, const string &id, long long int flight_hours)
{
    if (IS_User_Name_Present(name))
        return false;

    file.open(file_name, ios::app);
    if (!file.is_open())
        return false;

    file << id << "," << name << "," << password << "," << age << "," << email << "," << flight_hours << "\n";
    file.close();
    return true;
}

optional<UserInfo> FileHandler::Get_Hashed_Password(const string &target_username)
{
    file.open(file_name, ios::in);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file for reading." << endl;
        return nullopt;
    }

    string line;
    getline(file, line);

    while (getline(file, line))
    {
        stringstream ss(line);
        string id, name, password, age, email;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, password, ',');
        getline(ss, age, ',');
        getline(ss, email, ',');

        if (!name.empty() && name.front() == '"')
        {
            name.erase(0, 1);
        }
        if (!name.empty() && name.back() == '"')
        {
            name.erase(name.size() - 1);
        }
        if (name == target_username)
        {
            file.close();
            return UserInfo{id, password};
        }
    }

    file.close();
    return nullopt;
}

bool FileHandler::Save_Aircraft(const string &aircraft_id, const string &type, int capacity, bool status)
{
    file.open(file_name, ios::app);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file " << file_name << " for writing aircraft." << endl;
        return false;
    }

    file << aircraft_id << "," << type << "," << capacity << "," << status << "\n";
    file.close();
    return true;
}

bool FileHandler::Save_Flight(const string &flight_number, const string &origin, const string &destination, const string &departure_time, const string &arrival_time, const string &aircraft_id, const string &status, double price)
{
    file.open(file_name, ios::app);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file " << file_name << " for writing flight." << endl;
        return false;
    }
    file << flight_number << ","
         << origin << ","
         << destination << ","
         << departure_time << ","
         << arrival_time << ","
         << aircraft_id << ","
         << status << ","
         << price << "\n";
    file.close();
    return true;
}

bool FileHandler::Save_Reservation(const string &reservation_id, const string &passenger_id, const string &flight_number, const string &seat_id, const string &payment_id, const string &status)
{
    file.open(file_name, ios::app);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file " << file_name << " for writing reservation." << endl;
        return false;
    }

    file << reservation_id << ","
         << passenger_id << ","
         << flight_number << ","
         << seat_id << ","
         << payment_id << ","
         << status << "\n";

    file.close();
    return true;
}

bool FileHandler::Save_CrewFlight(const string &crew_id, const string &flight_id)
{
    file.open(file_name, ios::app);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file " << file_name << " for writing crew assignment." << endl;
        return false;
    }

    file << crew_id << "," << flight_id << "\n";
    file.close();
    return true;
}

bool FileHandler::Update_Reservation_Status(const string &reservation_id, const string &status)
{
    file.open(file_name, ios::in);
    if (!file.is_open())
        return false;

    string temporary_file_name = Get_Temp_File_Name(file_name);
    ofstream temporary_file(temporary_file_name);
    if (!temporary_file.is_open())
    {
        file.close();
        return false;
    }

    string line;
    bool updated = false;
    if (getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        temporary_file << line << "\n";
    }

    while (getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        stringstream stream(line);
        string current_id, passenger_id, flight_number, seat_id, payment_id, current_status;
        getline(stream, current_id, ',');
        getline(stream, passenger_id, ',');
        getline(stream, flight_number, ',');
        getline(stream, seat_id, ',');
        getline(stream, payment_id, ',');
        getline(stream, current_status, ',');

        if (current_id == reservation_id)
        {
            temporary_file << current_id << ',' << passenger_id << ',' << flight_number << ',' << seat_id << ',' << payment_id << ',' << status << "\n";
            updated = true;
        }
        else
        {
            temporary_file << line << "\n";
        }
    }

    file.close();
    temporary_file.close();

    if (!updated)
    {
        remove(temporary_file_name.c_str());
        return false;
    }

    remove(file_name.c_str());
    return rename(temporary_file_name.c_str(), file_name.c_str()) == 0;
}

bool FileHandler::Update_Aircraft_status(const string &aircraft_id, bool status)
{
    file.open(file_name, ios::in);
    if (!file.is_open())
        return false;

    string temporary_file_name = Get_Temp_File_Name(file_name);
    ofstream temporary_file(temporary_file_name);
    if (!temporary_file.is_open())
    {
        file.close();
        return false;
    }

    string line;
    bool updated = false;
    if (getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        temporary_file << line << "\n";
    }

    while (getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        stringstream stream(line);
        string current_id, type, capacity, current_status;
        getline(stream, current_id, ',');
        getline(stream, type, ',');
        getline(stream, capacity, ',');
        getline(stream, current_status, ',');

        if (current_id == aircraft_id)
        {
            temporary_file << current_id << ',' << type << ',' << capacity << ',' << status << "\n";
            updated = true;
        }
        else
        {
            temporary_file << line << "\n";
        }
    }

    file.close();
    temporary_file.close();

    if (!updated)
    {
        remove(temporary_file_name.c_str());
        return false;
    }

    remove(file_name.c_str());
    return rename(temporary_file_name.c_str(), file_name.c_str()) == 0;
}

bool FileHandler::Delete_Record(const string &target_id, int id_column_index)
{
    file.open(file_name, ios::in);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file for reading: " << file_name << endl;
        return false;
    }

    string temp_file_name = Get_Temp_File_Name(file_name);
    ofstream tempFile(temp_file_name);
    if (!tempFile.is_open())
    {
        file.close();
        return false;
    }

    string line;
    bool found_and_deleted = false;
    if (getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        tempFile << line << "\n";
    }

    while (getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        stringstream ss(line);
        string current_col;
        int col_counter = 0;
        string found_id = "";
        while (getline(ss, current_col, ','))
        {
            if (col_counter == id_column_index)
            {
                found_id = current_col;
                break;
            }
            col_counter++;
        }

        if (found_id == target_id)
        {
            found_and_deleted = true;
        }
        else
        {
            tempFile << line << "\n";
        }
    }

    file.close();
    tempFile.close();

    if (!found_and_deleted)
    {
        remove(temp_file_name.c_str());
        return false;
    }

    remove(file_name.c_str());
    return rename(temp_file_name.c_str(), file_name.c_str()) == 0;
}

std::vector<UserData> FileHandler::Load_Users()
{
    std::vector<UserData> users;
    file.open(file_name, std::ios::in);
    if (!file.is_open())
        return users;

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (line.empty())
            continue;

        std::stringstream ss(line);
        UserData user;
        std::string age_str;

        std::getline(ss, user.id, ',');
        std::getline(ss, user.name, ',');
        std::getline(ss, user.password, ',');
        std::getline(ss, age_str, ',');
        std::getline(ss, user.email, ',');

        if (!age_str.empty())
        {
            user.age = std::stoi(age_str);
        }

        users.push_back(user);
    }

    file.close();
    return users;
}

std::vector<CrewData> FileHandler::Load_Crew()
{
    std::vector<CrewData> crew_members;
    file.open(file_name, std::ios::in);
    if (!file.is_open())
        return crew_members;

    std::string line;
    std::getline(file, line);
    while (std::getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (line.empty())
            continue;

        std::stringstream stream(line);
        CrewData crew;
        std::string age_str, hours_str;
        std::getline(stream, crew.id, ',');
        std::getline(stream, crew.name, ',');
        std::getline(stream, crew.password, ',');
        std::getline(stream, age_str, ',');
        std::getline(stream, crew.email, ',');
        std::getline(stream, hours_str, ',');

        if (!age_str.empty())
            crew.age = std::stoi(age_str);
        if (!hours_str.empty())
            crew.flight_hours = std::stoll(hours_str);
        crew_members.push_back(crew);
    }

    file.close();
    return crew_members;
}

std::vector<AircraftData> FileHandler::Load_Aircraft()
{
    std::vector<AircraftData> fleet;
    file.open(file_name, std::ios::in);
    if (!file.is_open())
        return fleet;

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (line.empty())
            continue;

        std::stringstream ss(line);
        AircraftData plane;
        std::string capacity_str, status_str;

        std::getline(ss, plane.aircraft_id, ',');
        std::getline(ss, plane.type, ',');
        std::getline(ss, capacity_str, ',');

        if (!capacity_str.empty())
        {
            plane.capacity = std::stoi(capacity_str);
        }

        plane.status = true;
        if (std::getline(ss, status_str, ',') && !status_str.empty())
            plane.status = status_str == "1" || status_str == "true";

        fleet.push_back(plane);
    }

    file.close();
    return fleet;
}

std::vector<FlightData> FileHandler::Load_Flights()
{
    std::vector<FlightData> flights;
    file.open(file_name, std::ios::in);
    if (!file.is_open())
        return flights;

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (line.empty())
            continue;

        std::stringstream ss(line);
        FlightData flight;

        std::getline(ss, flight.flight_number, ',');
        std::getline(ss, flight.origin, ',');
        std::getline(ss, flight.destination, ',');
        std::getline(ss, flight.departure_time, ',');
        std::getline(ss, flight.arrival_time, ',');
        std::getline(ss, flight.aircraft_id, ',');
        std::getline(ss, flight.status, ',');

        std::string price_str;
        std::getline(ss, price_str, ',');
        if (!price_str.empty())
        {
            flight.price = std::stod(price_str);
        }

        flights.push_back(flight);
    }

    file.close();
    return flights;
}

std::vector<ReservationData> FileHandler::Load_Reservations()
{
    std::vector<ReservationData> reservations;
    file.open(file_name, std::ios::in);
    if (!file.is_open())
        return reservations;

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (line.empty())
            continue;

        std::stringstream ss(line);
        ReservationData res;

        std::getline(ss, res.reservation_id, ',');
        std::getline(ss, res.passenger_id, ',');
        std::getline(ss, res.flight_number, ',');
        std::getline(ss, res.seat_id, ',');
        std::getline(ss, res.payment_id, ',');
        std::getline(ss, res.status, ',');

        reservations.push_back(res);
    }

    file.close();
    return reservations;
}

bool FileHandler::Save_Passenger(const string &name, const string &password, const string &email, int age, const string &id, long long int credit, int points)
{
    if (IS_User_Name_Present(name))
    {
        return false;
    }

    file.open(file_name, ios::app);
    if (!file.is_open())
    {
        return false;
    }
    file << id << "," << name << "," << password << "," << age << "," << email << "," << credit << "," << points << "\n";
    file.close();
    return true;
}

std::vector<PassengerData> FileHandler::Load_Passengers()
{
    std::vector<PassengerData> passengers;
    file.open(file_name, std::ios::in);
    if (!file.is_open())
        return passengers;

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (line.empty())
            continue;

        std::stringstream ss(line);
        PassengerData passenger;
        std::string age_str, credit_str, points_str;

        std::getline(ss, passenger.id, ',');
        std::getline(ss, passenger.name, ',');
        std::getline(ss, passenger.password, ',');
        std::getline(ss, age_str, ',');
        std::getline(ss, passenger.email, ',');
        std::getline(ss, credit_str, ',');
        std::getline(ss, points_str, ',');

        if (!age_str.empty())
            passenger.age = std::stoi(age_str);
        if (!credit_str.empty())
            passenger.credit = std::stoll(credit_str);
        if (!points_str.empty())
            passenger.points = std::stoi(points_str);

        passengers.push_back(passenger);
    }

    file.close();
    return passengers;
}

bool FileHandler::Update_Passenger_Credit(const string &passenger_id, long long int new_credit)
{
    file.open(file_name, ios::in);
    if (!file.is_open())
        return false;

    string temporary_file_name = Get_Temp_File_Name(file_name);
    ofstream temporary_file(temporary_file_name);
    if (!temporary_file.is_open())
    {
        file.close();
        return false;
    }

    string line;
    bool updated = false;

    if (getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        temporary_file << line << "\n";
    }

    while (getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        stringstream stream(line);
        string id, name, password, age, email, credit, points;
        getline(stream, id, ',');
        getline(stream, name, ',');
        getline(stream, password, ',');
        getline(stream, age, ',');
        getline(stream, email, ',');
        getline(stream, credit, ',');
        getline(stream, points, ',');

        if (id == passenger_id)
        {
            if (points.empty())
                points = "0";
            temporary_file << id << ',' << name << ',' << password << ',' << age << ',' << email << ',' << new_credit << ',' << points << "\n";
            updated = true;
        }
        else
        {
            temporary_file << line << "\n";
        }
    }

    file.close();
    temporary_file.close();

    if (!updated)
    {
        remove(temporary_file_name.c_str());
        return false;
    }

    remove(file_name.c_str());
    return rename(temporary_file_name.c_str(), file_name.c_str()) == 0;
}

bool FileHandler::Update_Points(const string &passenger_id, int new_points)
{
    if (new_points < 0)
        return false;

    file.open(file_name, ios::in);
    if (!file.is_open())
        return false;

    string temporary_file_name = Get_Temp_File_Name(file_name);
    ofstream temporary_file(temporary_file_name);
    if (!temporary_file.is_open())
    {
        file.close();
        return false;
    }

    string line;
    bool updated = false;
    if (getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        temporary_file << line << "\n";
    }

    while (getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        stringstream stream(line);
        string id, name, password, age, email, credit, points;
        getline(stream, id, ',');
        getline(stream, name, ',');
        getline(stream, password, ',');
        getline(stream, age, ',');
        getline(stream, email, ',');
        getline(stream, credit, ',');
        getline(stream, points, ',');

        if (id == passenger_id)
        {
            if (credit.empty())
                credit = "0";
            temporary_file << id << ',' << name << ',' << password << ',' << age << ',' << email << ',' << credit << ',' << new_points << "\n";
            updated = true;
        }
        else
        {
            temporary_file << line << "\n";
        }
    }

    file.close();
    temporary_file.close();
    if (!updated)
    {
        remove(temporary_file_name.c_str());
        return false;
    }

    remove(file_name.c_str());
    return rename(temporary_file_name.c_str(), file_name.c_str()) == 0;
}

bool FileHandler::Update_Crew_Flight_Hours(const string &crew_id, long long int flight_hours)
{
    file.open(file_name, ios::in);
    if (!file.is_open())
        return false;

    string temporary_file_name = Get_Temp_File_Name(file_name);
    ofstream temporary_file(temporary_file_name);
    if (!temporary_file.is_open())
    {
        file.close();
        return false;
    }

    string line;
    bool updated = false;
    if (getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        temporary_file << line << "\n";
    }

    while (getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        stringstream stream(line);
        string id, name, password, age, email;
        getline(stream, id, ',');
        getline(stream, name, ',');
        getline(stream, password, ',');
        getline(stream, age, ',');
        getline(stream, email, ',');

        if (id == crew_id)
        {
            temporary_file << id << ',' << name << ',' << password << ',' << age << ',' << email << ',' << flight_hours << "\n";
            updated = true;
        }
        else
        {
            temporary_file << line << "\n";
        }
    }

    file.close();
    temporary_file.close();
    if (!updated)
    {
        remove(temporary_file_name.c_str());
        return false;
    }

    remove(file_name.c_str());
    return rename(temporary_file_name.c_str(), file_name.c_str()) == 0;
}

FileHandler::~FileHandler()
{
    if (file.is_open())
    {
        file.close();
    }
}