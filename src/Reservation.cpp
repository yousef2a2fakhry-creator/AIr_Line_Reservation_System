#include "../inc/Reservation.h"

#include "../inc/AirCraft.h"
#include "../inc/Flight.h"
#include "../inc/Passenger.h"
#include "../inc/Payment.h"

using namespace std;

Reservation::Reservation(string reservation_id,
                         shared_ptr<Passenger> passenger,
                         shared_ptr<Flight> flight,
                         string seat_id,
                         shared_ptr<Payment> payment,
                         string status)
    : reservation_id(reservation_id), passenger(passenger), flight(flight), payment(payment), seat_id(seat_id), status(status) {}

const string &Reservation::getReservationId() const
{
    return reservation_id;
}

shared_ptr<Passenger> Reservation::getPassenger() const
{
    return passenger;
}

shared_ptr<Flight> Reservation::getFlight() const
{
    return flight;
}

const string &Reservation::getSeatId() const
{
    return seat_id;
}

shared_ptr<Payment> Reservation::getPayment() const
{
    return payment;
}

const string &Reservation::getStatus() const
{
    return status;
}

bool Reservation::confirmReservation(double &user_credit)
{
    if (status == "Confirmed" || passenger == nullptr || flight == nullptr ||
        payment == nullptr || flight->getAircraft() == nullptr)
    {
        return false;
    }

    if (!payment->processPayment(user_credit))
    {
        return false;
    }

    shared_ptr<AirCraft> aircraft = flight->getAircraft();
    if (!aircraft->reserveSeat(seat_id))
    {
        payment->processRefund(user_credit);
        return false;
    }

    if (!flight->addPassenger(passenger))
    {
        aircraft->releaseSeat(seat_id);
        payment->processRefund(user_credit);
        return false;
    }

    status = "Confirmed";
    return true;
}

bool Reservation::cancelReservation(double &user_credit)
{
    if (status != "Confirmed" || passenger == nullptr || flight == nullptr ||
        payment == nullptr || flight->getAircraft() == nullptr)
    {
        return false;
    }

    if (!payment->processRefund(user_credit))
    {
        return false;
    }

    shared_ptr<AirCraft> aircraft = flight->getAircraft();
    if (!aircraft->releaseSeat(seat_id))
    {
        return false;
    }

    if (!flight->deletePassenger(passenger->getPassengerId()))
    {
        aircraft->reserveSeat(seat_id);
        return false;
    }

    status = "Canceled";
    return true;
}
