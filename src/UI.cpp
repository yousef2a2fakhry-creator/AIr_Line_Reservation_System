#include "../inc/UI.h"
#include "../inc/System.h"
#include <iostream>
#include <limits>
#include <optional>
#include <memory>
#include "../inc/FileHandler.h"
#include "../inc/Flight.h"
#include "../inc/Reservation.h"

using namespace std;

UI::UI(System &shared_system) : sys(shared_system) {}

void UI::clearInputBuffer()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string UI::readLine(const string &prompt)
{
    cout << prompt;
    string value;
    getline(cin, value);
    return value;
}

double UI::readNonNegativeAmount(const string &prompt)
{
    double value = 0.0;
    cout << prompt;
    while (!(cin >> value) || value < 0.0)
    {
        clearInputBuffer();
        cout << "Invalid amount. Enter a non-negative number: ";
    }
    clearInputBuffer();
    return value;
}

size_t UI::readPositiveSize(const string &prompt)
{
    size_t value = 0;
    cout << prompt;
    while (!(cin >> value) || value == 0)
    {
        clearInputBuffer();
        cout << "Invalid value. Enter a positive number: ";
    }
    clearInputBuffer();
    return value;
}

void UI::displayDateTimeFormatNotice() const
{
    cout << "Enter dates and times in the format YYYY-MM-DD HH:MM "
         << "(example: 2026-09-23 14:30).\n";
    cout << "For date-only search, enter YYYY-MM-DD; press Enter to skip a filter.\n";
}

void UI::handleBooking(const string &passengerId)
{
    string flight_number = readLine("Enter flight number: ");
    if (!sys.showSeats(flight_number))
    {
        cout << "\nThere is not flight with this number\n";
    }
    string seat_id = readLine("Enter seat ID: ");
    if (sys.makeReservation(passengerId, flight_number, seat_id))
        cout << "\nReservation confirmed successfully.\n";
    else
        cout << "\nUnable to create reservation. Check the flight, seat, and passenger balance.\n";
}

void UI::handlePassengerCredit(const string &passengerId)
{
    size_t amount = readPositiveSize("Enter the amount to add to your wallet: ");
    if (sys.addPassengerCredit(passengerId, static_cast<long long int>(amount)))
        cout << "\nCredit added to your wallet successfully.\n";
    else
        cout << "\nUnable to add credit to your wallet.\n";
}

void UI::displayReservations(const string &passengerId)
{
    auto reservations = sys.getReservationsForPassenger(passengerId);
    if (reservations.empty())
    {
        cout << "\nNo reservations found.\n";
        return;
    }

    cout << "\n--- RESERVATIONS ---\n";
    for (const auto &reservation : reservations)
    {
        cout << "Reservation ID: " << reservation->getReservationId() << '\n'
             << "Flight: " << reservation->getFlight()->getFlightNumber() << '\n'
             << "Seat: " << reservation->getSeatId() << '\n'
             << "Status: " << reservation->getStatus() << "\n\n";
    }
}

void UI::searchPassengerReservations(const string &passengerId)
{
    auto reservations = sys.getReservationsForPassenger(passengerId);
    if (reservations.empty())
    {
        cout << "\nNo reservations found.\n";
        return;
    }

    string search_value = readLine("Enter reservation ID or flight number: ");
    bool found = false;
    for (const auto &reservation : reservations)
    {
        if (!reservation || !reservation->getFlight())
            continue;

        if (reservation->getReservationId() == search_value ||
            reservation->getFlight()->getFlightNumber() == search_value)
        {
            cout << "\n--- RESERVATION FOUND ---\n"
                 << "Reservation ID: " << reservation->getReservationId() << '\n'
                 << "Flight: " << reservation->getFlight()->getFlightNumber() << '\n'
                 << "Route: " << reservation->getFlight()->getOrigin() << " -> "
                 << reservation->getFlight()->getDestination() << '\n'
                 << "Seat: " << reservation->getSeatId() << '\n'
                 << "Status: " << reservation->getStatus() << "\n";
            found = true;
        }
    }

    if (!found)
        cout << "\nNo reservation matched that ID or flight number.\n";
}

void UI::handleReservationCancellation(const string &requesterId, bool agentAction)
{
    string reservation_id = readLine(agentAction ? "Enter reservation ID to cancel: " : "Enter your reservation ID to cancel: ");
    if (sys.cancelReservation(requesterId, reservation_id))
        cout << "\nReservation canceled and payment refunded successfully.\n";
    else
        cout << "\nUnable to cancel reservation. Verify the ID, ownership, and reservation status.\n";
}

void UI::addAircraftEntry(const string &adminId)
{
    string aircraft_id = readLine("Aircraft ID: ");
    string type = readLine("Aircraft type: ");
    size_t capacity = readPositiveSize("Capacity: ");
    string status_input;
    bool status;

    do
    {
        status_input = readLine("Aircraft available (true/false): ");
        status = status_input == "true" || status_input == "1";
        if (!status && status_input != "false" && status_input != "0")
            cout << "Invalid status. Enter true or false.\n";
    } while (!status && status_input != "false" && status_input != "0");

    if (sys.addAircraft(adminId, aircraft_id, type, capacity, status))
        cout << "\nAircraft added successfully.\n";
    else
        cout << "\nUnable to add aircraft. Check admin access or duplicate aircraft ID.\n";
}

void UI::interact()
{
    if (!sys.loadData())
    {
        throw runtime_error("Something went wrong when openeing application \n");
    }
    bool running = true;
    int option = 0;

    while (running)
    {
        cout << "\n====================================\n";
        cout << "    AIRLINE RESERVATION SYSTEM      \n";
        cout << "====================================\n";
        cout << "1. Login\n";
        cout << "2. Register Passenger\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";

        if (!(cin >> option))
        {
            cout << "Invalid input. Please enter a number.\n";
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (option)
        {
        case 1:
            handleLogin();
            break;
        case 2:
            handleRegistration();
            break;
        case 3:
            cout << "Exiting system...\n";
            running = false;
            throw runtime_error("Good Bye See You Soon\n");
            break;
        default:
            cout << "Invalid choice! Please select 1, 2, or 3.\n";
        }
    }
}

void UI::handleRegistration()
{
    string name, password, email;
    int age = 0;

    cout << "\n=== Passenger Registration ===\n";

    name = readLine("Enter Full Name: ");
    email = readLine("Enter Email: ");
    password = readLine("Enter Password: ");

    cout << "Enter Age: ";
    while (!(cin >> age) || age <= 0)
    {
        cout << "Invalid age. Please enter a valid number: ";
        clearInputBuffer();
    }
    clearInputBuffer();

    if (sys.registerUser(name, password, email, age))
    {
        cout << "\n[Success] Registration complete! You can now log in.\n";
    }
    else
    {
        cerr << "\nUser Name is already present, try another user name.\n";
    }
}

void UI::handleLogin()
{
    int login_choice = 0;
    string name, password;

    cout << "\n=== User Login ===\n";
    cout << "1. Passenger\n";
    cout << "2. Booking Agent\n";
    cout << "3. Admin\n";
    cout << "4. Crew\n";
    cout << "Select Role: ";

    if (!(cin >> login_choice) || login_choice < 1 || login_choice > 4)
    {
        cout << "Invalid role selection.\n";
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    name = readLine("Enter Name: ");
    password = readLine("Enter Password: ");

    optional<Udata> state = sys.Login(login_choice, name, password);
    if (!state.has_value())
    {
        cout << "\n[Error] Login failed. Invalid username or password.\n";
        return;
    }

    string userId = state->id;

    if (state->requires_password_change)
    {
        cout << "\n[Security Notice] You are using a default password.\n";
        string new_password = readLine("You have to change your password now. Please enter a new password: ");

        sys.UpdateUserPassword(login_choice, userId, new_password);
        cout << "[Success] Password updated successfully.\n";
    }

    cout << "\n[Success] Welcome! Logged in with ID: " << userId << "\n";

    switch (login_choice)
    {
    case 1:
        passengerDashboard(userId);
        break;
    case 2:
        agentDashboard(userId);
        break;
    case 3:
        adminDashboard(userId);
        break;
    case 4:
        crewDashboard(userId);
        break;
    }
}

void UI::passengerDashboard(const string &userId)
{
    bool in_session = true;
    int choice = 0;

    auto current_user = sys.getUser(userId);
    if (current_user)
    {
        current_user->display();
    }

    while (in_session)
    {
        cout << "\n--- PASSENGER DASHBOARD ---\n";
        cout << "1. Search Flights\n";
        cout << "2. Book Flight & Select Seat\n";
        cout << "3. View My Bookings\n";
        cout << "4. Search Reservation\n";
        cout << "5. Cancel Reservation\n";
        cout << "6. Add Credit to Wallet\n";
        cout << "7. Logout\n";
        cout << "Choice: ";

        if (!(cin >> choice))
        {
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            handleFlightSearch();
            break;
        case 2:
        {
            handleBooking(userId);
            break;
        }
        case 3:
            displayReservations(userId);
            break;
        case 4:
            searchPassengerReservations(userId);
            break;
        case 5:
            handleReservationCancellation(userId, false);
            break;
        case 6:
            handlePassengerCredit(userId);
            break;
        case 7:
            cout << "Logging out of Passenger Dashboard...\n";
            in_session = false;
            break;
        default:
            cout << "Invalid option. Please try again.\n";
        }
    }
}

void UI::agentDashboard(const string &userId)
{
    bool in_session = true;
    int choice = 0;

    auto current_user = sys.getUser(userId);
    if (current_user)
    {
        current_user->display();
    }

    while (in_session)
    {
        cout << "\n--- BOOKING AGENT DASHBOARD ---\n";
        cout << "1. Search Flights\n";
        cout << "2. Book Reservation for Passenger\n";
        cout << "3. View Passenger Reservations\n";
        cout << "4. Cancel Booking\n";
        cout << "5. Logout\n";
        cout << "Choice: ";

        if (!(cin >> choice))
        {
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            handleFlightSearch();
            break;
        case 2:
        {
            string passenger_username = readLine("Enter passenger username: ");
            FileHandler file("data/Passengers.csv");
            optional<UserInfo> userInfo = file.Get_Hashed_Password(passenger_username);

            if (userInfo.has_value())
            {
                handleBooking(userInfo->id);
            }
            else
            {
                cout << "\nError: Passenger with username '" << passenger_username << "' not found.\n";
            }
            break;
        }
        case 3:
        {
            string passenger_username = readLine("Enter passenger username: ");
            FileHandler file("data/Passengers.csv");
            optional<UserInfo> userInfo = file.Get_Hashed_Password(passenger_username);

            if (userInfo.has_value())
            {
                displayReservations(userInfo->id);
            }
            else
            {
                cout << "\nError: Passenger with username '" << passenger_username << "' not found.\n";
            }
            break;
        }
        case 4:
            handleReservationCancellation(userId, true);
            break;
        case 5:
            cout << "Logging out of Agent Dashboard...\n";
            in_session = false;
            break;
        default:
            cout << "Invalid option. Please try again.\n";
        }
    }
}

void UI::crewDashboard(const string &userId)
{
    bool in_session = true;
    int choice = 0;

    auto current_user = sys.getUser(userId);
    if (current_user)
    {
        current_user->display();
    }

    while (in_session)
    {
        cout << "\n--- CREW DASHBOARD ---\n";
        cout << "1. View assigned flight duties\n";
        cout << "2. View crew profile\n";
        cout << "3. Logout\n";
        cout << "Choice: ";

        if (!(cin >> choice))
        {
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice)
        {
        case 1:
        {
            auto assigned_flights = sys.getCrewFlights(userId);
            if (assigned_flights.empty())
            {
                cout << "\nNo flights are assigned to you.\n";
                break;
            }

            cout << "\n--- YOUR ASSIGNED FLIGHTS ---\n";
            for (const auto &flight : assigned_flights)
            {
                cout << "Flight: " << flight->getFlightNumber() << "\n"
                     << "Route: " << flight->getOrigin() << " -> " << flight->getDestination() << "\n"
                     << "Departure: " << flight->getDepartureTime() << "\n"
                     << "Arrival: " << flight->getArrivalTime() << "\n"
                     << "Status: " << flight->getStatus() << "\n\n";
            }
            break;
        }
        case 2:
            if (current_user)
            {
                current_user->display();
            }
            break;
        case 3:
            cout << "Logging out of Crew Dashboard...\n";
            in_session = false;
            break;
        default:
            cout << "Invalid option. Please try again.\n";
        }
    }
}

void UI::adminDashboard(const string &userId)
{
    bool in_session = true;
    int choice = 0;

    auto current_user = sys.getUser(userId);
    if (current_user)
    {
        current_user->display();
    }

    while (in_session)
    {
        cout << "\n--- ADMIN DASHBOARD ---\n";
        cout << "1. Schedule New Flight\n";
        cout << "2. Add Aircraft\n";
        cout << "3. Manage User Accounts\n";
        cout << "4. Add Crew Member To The Flight\n";
        cout << "5. Logout\n";
        cout << "Choice: ";

        if (!(cin >> choice))
        {
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice)
        {
        case 1:
        {
            string flight_number, origin, destination, departure_time, arrival_time, aircraft_id;
            displayDateTimeFormatNotice();
            flight_number = readLine("Flight number: ");
            origin = readLine("Origin: ");
            destination = readLine("Destination: ");
            departure_time = readLine("Departure time [YYYY-MM-DD HH:MM]: ");
            arrival_time = readLine("Arrival time [YYYY-MM-DD HH:MM]: ");
            aircraft_id = readLine("Aircraft ID: ");
            double price = readNonNegativeAmount("Price: ");

            if (sys.scheduleFlight(userId, flight_number, origin, destination,
                                   departure_time, arrival_time, aircraft_id, price))
                cout << "\nFlight scheduled successfully.\n";
            else
                cout << "\nUnable to schedule flight. Check admin access, aircraft ID, or duplicate flight number.\n";
            break;
        }
        case 2:
            addAircraftEntry(userId);
            break;
        case 3:
        {
            cout << "\n--- ACCOUNT MANAGEMENT ---\n";
            cout << "1. Create Passenger\n";
            cout << "2. Create Crew Member\n";
            cout << "3. Create Booking Agent\n";
            cout << "4. Create Admin\n";
            cout << "5. Delete User\n";
            cout << "6. Return to Admin Dashboard\n";
            cout << "Choice: ";

            if (!(cin >> choice))
            {
                clearInputBuffer();
                break;
            }
            clearInputBuffer();

            if (choice >= 1 && choice <= 4)
            {
                string newName, newPassword, newEmail;
                int newAge;

                newName = readLine("Enter Name: ");
                newPassword = readLine("Enter Password: ");
                newEmail = readLine("Enter Email: ");

                cout << "Enter Age: ";
                while (!(cin >> newAge))
                {
                    clearInputBuffer();
                    cout << "Invalid input. Please enter a valid number for age: ";
                }
                clearInputBuffer();

                bool success = false;
                if (choice == 1)
                {
                    success = sys.registerUser(newName, newPassword, newEmail, newAge);
                }
                else if (choice == 2)
                {
                    success = sys.createStaffUser(userId, "Crew", newName, newPassword, newEmail, newAge);
                }
                else if (choice == 3)
                {
                    success = sys.createStaffUser(userId, "BookingAgent", newName, newPassword, newEmail, newAge);
                }
                else if (choice == 4)
                {
                    success = sys.createStaffUser(userId, "Admin", newName, newPassword, newEmail, newAge);
                }
                if (success)
                    cout << "\nAccount for '" << newName << "' created successfully!\n";
                else
                    cout << "\nFailed to create account. Action unauthorized or system error.\n";
            }
            else if (choice == 5)
            {
                cout << "\n--- DELETE USER ---\n";
                cout << "Which type of user do you want to delete?\n";
                cout << "1. Passenger\n";
                cout << "2. Crew Member\n";
                cout << "3. Booking Agent\n";
                cout << "4. Admin\n";
                cout << "Choice: ";

                int deleteChoice;
                if (!(cin >> deleteChoice))
                {
                    clearInputBuffer();
                    cout << "Invalid choice.\n";
                    break;
                }
                clearInputBuffer();
                string filename = "";
                if (deleteChoice == 1)
                    filename = "data/Passengers.csv";
                else if (deleteChoice == 2)
                    filename = "data/Crew.csv";
                else if (deleteChoice == 3)
                    filename = "data/BookingAgent.csv";
                else if (deleteChoice == 4)
                    filename = "data/Admin.csv";
                else
                {
                    cout << "Invalid user type selected.\n";
                    break;
                }

                string targetUsername = readLine("Enter the username of the user to delete: ");

                FileHandler file(filename);
                optional<UserInfo> userInfo = file.Get_Hashed_Password(targetUsername);
                if (userInfo.has_value() && file.Delete_Record(targetUsername, 1))
                {
                    cout << "\nSuccess: User with username '" << targetUsername << "' was deleted from " << filename << ".\n";
                    sys.removeUser(current_user->getID(), userInfo->id);
                }
                else
                {
                    cout << "\nError: Could not find or delete user with username '" << targetUsername << "' in " << filename << ".\n";
                }
            }
            else if (choice != 6)
            {
                cout << "Invalid account management option.\n";
            }
            break;
        }
        case 4:
        {
            string crew_username = readLine("Crew member username: ");
            string flight_id = readLine("Flight ID: ");

            FileHandler crew_file("data/Crew.csv");
            optional<UserInfo> crew_info = crew_file.Get_Hashed_Password(crew_username);
            if (!crew_info.has_value())
            {
                cout << "\nUnable to find a crew member with that username.\n";
                break;
            }

            if (sys.assignCrewMemberToFlight(crew_info->id, flight_id))
                cout << "\nCrew member assigned to the flight successfully.\n";
            else
                cout << "\nUnable to assign crew member. Check the flight ID and the crew member's availability.\n";
            break;
        }
        case 5:
            cout << "Logging out of Admin Dashboard...\n";
            in_session = false;
            break;
        default:
            cout << "Invalid option. Please try again.\n";
        }
    }
}

void UI::handleFlightSearch()
{
    string origin, destination, dep_date;
    double max_price = -1.0;
    char choice;

    cout << "\n=== SEARCH FLIGHTS ===\n";
    displayDateTimeFormatNotice();

    origin = readLine("Enter Origin (or press Enter to skip): ");
    destination = readLine("Enter Destination (or press Enter to skip): ");
    dep_date = readLine("Enter Departure Date [YYYY-MM-DD] (or press Enter to skip): ");

    cout << "Filter by maximum price? (y/n): ";
    cin >> choice;
    if (choice == 'y' || choice == 'Y')
    {
        max_price = readNonNegativeAmount("Enter Max Price ($): ");
    }
    else
    {
        clearInputBuffer();
    }
    auto matching_flights = sys.searchFlights(origin, destination, max_price, dep_date);

    if (matching_flights.empty())
    {
        cout << "\n[Results] No flights found matching your search criteria.\n";
        return;
    }
    cout << "\nSort results by:\n1. Price (Low to High)\n2. Departure Time\n3. No Sorting\nChoice: ";
    int sort_choice = 0;
    cin >> sort_choice;
    clearInputBuffer();

    if (sort_choice == 1)
    {
        sys.sortByPrice(matching_flights, true);
    }
    else if (sort_choice == 2)
    {
        sys.sortByDepartureTime(matching_flights);
    }
    cout << "\n[Results] " << matching_flights.size() << " flight(s) found.\n";
    for (const auto &f : matching_flights)
    {
        if (f)
        {
            f->display();
        }
    }
}