#ifndef UI_H
#define UI_H
#include <string>
class System;

class UI
{
private:
    System &sys;

    void clearInputBuffer();
    std::string readLine(const std::string &prompt);
    double readNonNegativeAmount(const std::string &prompt);
    std::size_t readPositiveSize(const std::string &prompt);
    void displayDateTimeFormatNotice() const;
    void handleRegistration();
    void handleLogin();
    void passengerDashboard(const std::string &userId);
    void agentDashboard(const std::string &userId);
    void crewDashboard(const std::string &userId);
    void adminDashboard(const std::string &userId);
    void handleFlightSearch();
    void handleBooking(const std::string &passengerId);
    void handlePassengerCredit(const std::string &passengerId);
    void displayReservations(const std::string &passengerId);
    void searchPassengerReservations(const std::string &passengerId);
    void handleReservationCancellation(const std::string &requesterId, bool agentAction);
    void addAircraftEntry(const std::string &adminId);

public:
    explicit UI(System &shared_system);
    void interact();
};

#endif