#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <cstddef>
#include <map>
#include <string>

class Seat
{
private:
    std::string id;
    bool available;

public:
    explicit Seat(std::string id, bool available = true);
    const std::string &getId() const;
    bool isAvailable() const;
    void setAvailable(bool available);
};

class AirCraft
{
private:
    std::string aircraft_id;
    std::string aircraft_type;
    std::size_t capacity;
    bool available;
    std::map<std::string, Seat> seats;

public:
    AirCraft(std::string aircraft_id,
             std::string aircraft_type,
             std::size_t capacity,
             bool available = true);

    const std::string &getId() const;
    const std::string &getType() const;
    std::size_t getCapacity() const;
    bool isAvailable() const;
    void setAvailable(bool available);

    const std::map<std::string, Seat> &getSeats() const;
    bool addSeat(const std::string &seat_id);
    bool selectSeat(const std::string &seat_id);
    bool reserveSeat(const std::string &seat_id);
    bool releaseSeat(const std::string &seat_id);
    Seat *findSeat(const std::string &seat_id);

public:
    void generateSeats();        
    void displaySeatMap() const; 
};

#endif