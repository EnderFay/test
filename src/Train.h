#ifndef TRAIN_H
#define TRAIN_H

#include <string>
#include <vector>
#include <chrono>
#include "Ticket.h" // Assuming Ticket class exists

enum class TrainStatus {
    SCHEDULED,
    DEPARTED,
    ARRIVED
};

class Train {
public:
    int trainNumber;
    int capacity;
    
    std::string currentStation;
    std::string destination;
    std::chrono::system_clock::time_point departureTime;
    std::string origin;
    std::chrono::system_clock::duration travelDuration;
    TrainStatus status;
    std::vector<Ticket> tickets;
    double speed;
    double distanceTraveled;

    Train(int trainNumber, int capacity, const std::string& origin, const std::string& destination,
          const std::chrono::system_clock::time_point& departureTime,
          const std::chrono::system_clock::duration& travelDuration, double speed);
    
    int getTrainNumber() const {
        return trainNumber;
    }
    int getAvailableSeats() const;
    bool buyTicket(const Ticket& ticket);
    bool returnTicket(int ticketId);
    // Update train's position based on elapsed time
    void updatePosition(const std::chrono::system_clock::duration& elapsedTime);

    friend std::ostream& operator<<(std::ostream& os, const Train& train);
};

#endif // TRAIN_H