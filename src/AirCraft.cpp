#include "../inc/AirCraft.h"
#include <iostream>
using namespace std;

Seat::Seat(string id, bool available) : id(id), available(available) {}

const string &Seat::getId() const
{
    return id;
}

bool Seat::isAvailable() const
{
    return available;
}

void Seat::setAvailable(bool new_available)
{
    available = new_available;
}

AirCraft::AirCraft(string aircraft_id,
                   string aircraft_type,
                   size_t capacity,
                   bool available)
    : aircraft_id(aircraft_id),
      aircraft_type(aircraft_type),
      capacity(capacity),
      available(available)
{
    generateSeats();
}

const string &AirCraft::getId() const
{
    return aircraft_id;
}

const string &AirCraft::getType() const
{
    return aircraft_type;
}

size_t AirCraft::getCapacity() const
{
    return capacity;
}

bool AirCraft::isAvailable() const
{
    return available;
}

void AirCraft::setAvailable(bool new_available)
{
    available = new_available;
}

const map<string, Seat> &AirCraft::getSeats() const
{
    return seats;
}

bool AirCraft::addSeat(const string &seat_id)
{
    if (seats.size() >= capacity || seats.find(seat_id) != seats.end())
    {
        return false;
    }
    return seats.emplace(seat_id, Seat(seat_id)).second;
}

bool AirCraft::selectSeat(const string &seat_id)
{
    Seat *seat = findSeat(seat_id);
    if (seat == nullptr || !seat->isAvailable())
    {
        return false;
    }
    seat->setAvailable(false);
    return true;
}

bool AirCraft::reserveSeat(const string &seat_id)
{
    return selectSeat(seat_id);
}

bool AirCraft::releaseSeat(const string &seat_id)
{
    Seat *seat = findSeat(seat_id);
    if (seat == nullptr || seat->isAvailable())
    {
        return false;
    }
    seat->setAvailable(true);
    return true;
}

Seat *AirCraft::findSeat(const string &seat_id)
{
    auto seat_iterator = seats.find(seat_id);
    if (seat_iterator != seats.end())
    {
        return &seat_iterator->second;
    }
    return nullptr;
}

void AirCraft::generateSeats()
{
    int seats_per_row = 6;
    int num_rows = capacity / seats_per_row;
    int remaining_seats = capacity % seats_per_row;

    for (int row = 1; row <= num_rows; ++row)
    {
        for (char col = 'A'; col < 'A' + seats_per_row; ++col)
        {
            string seat_id = to_string(row) + col;
            addSeat(seat_id);
        }
    }
    if (remaining_seats > 0)
    {
        int final_row = num_rows + 1;
        for (char col = 'A'; col < 'A' + remaining_seats; ++col)
        {
            string seat_id = to_string(final_row) + col;
            addSeat(seat_id);
        }
    }
}

void AirCraft::displaySeatMap() const
{
    cout << "\n--- SEAT MAP (Aircraft: " << aircraft_id << ") ---\n";
    cout << "Format: [ID] = Available | [XX] = Taken\n\n";

    int seats_per_row = 6;
    int total_rows = (capacity / seats_per_row) + (capacity % seats_per_row != 0 ? 1 : 0);

    for (int row = 1; row <= total_rows; ++row)
    {
        if (row < 10)
            cout << "0" << row << " ";
        else
            cout << row << " ";
        for (char col = 'A'; col < 'A' + seats_per_row; ++col)
        {
            string seat_id = to_string(row) + col;
            auto seat_iterator = seats.find(seat_id);
            if (seat_iterator != seats.end())
            {
                if (seat_iterator->second.isAvailable())
                {
                    cout << "[" << seat_id;
                    if (seat_id.length() == 2)
                        cout << " ";
                    cout << "] ";
                }
                else
                {
                    cout << "[ XX ] ";
                }
            }
            else
            {
                cout << "       ";
            }
            if (col == 'C')
            {
                cout << " ||  ";
            }
        }
        cout << "\n";
    }
    cout << "--------------------------------------\n";
}
