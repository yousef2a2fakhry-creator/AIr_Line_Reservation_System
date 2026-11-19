#ifndef RESERVATION_H
#define RESERVATION_H

#include <memory>
#include <string>

class Passenger;
class Flight;
class Payment;

class Reservation
{
private:
    std::string reservation_id;
    std::shared_ptr<Passenger> passenger;
    std::shared_ptr<Flight> flight;
    std::shared_ptr<Payment> payment;
    std::string seat_id;
    std::string status;

public:
    Reservation(std::string reservation_id,
                std::shared_ptr<Passenger> passenger,
                std::shared_ptr<Flight> flight,
                std::string seat_id,
                std::shared_ptr<Payment> payment,
                std::string status = "Waitlisted");

    const std::string &getReservationId() const;
    std::shared_ptr<Passenger> getPassenger() const;
    std::shared_ptr<Flight> getFlight() const;
    const std::string &getSeatId() const;
    std::shared_ptr<Payment> getPayment() const;
    const std::string &getStatus() const;

    bool confirmReservation(double &user_credit);
    bool cancelReservation(double &user_credit);
};

#endif
