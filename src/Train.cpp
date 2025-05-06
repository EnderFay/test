#include "Train.h"
#include <iostream>
#include <cmath>

Train::Train(int trainNumber, int capacity, const std::string& origin, const std::string& destination,
             const std::chrono::system_clock::time_point& departureTime,
             const std::chrono::system_clock::duration& travelDuration, double speed)
    : trainNumber(trainNumber), capacity(capacity), destination(destination), departureTime(departureTime), origin(origin), 
       travelDuration(travelDuration), status(TrainStatus::SCHEDULED),
      speed(speed), distanceTraveled(0.0) {
    currentStation = origin; // Train starts at the origin
}

int Train::getAvailableSeats() const {
    return capacity - tickets.size();
}

bool Train::buyTicket(const Ticket& ticket) {
    if (getAvailableSeats() > 0) {
        tickets.push_back(ticket);
        return true;
    }
    return false;
}

bool Train::returnTicket(int ticketId) {
    for (size_t i = 0; i < tickets.size(); ++i) {
        if (tickets[i].ticketId == ticketId) { // Assuming Ticket has a ticketId
            tickets.erase(tickets.begin() + i);
            return true;
        }
    }
    return false;
}

void Train::updatePosition(const std::chrono::system_clock::duration& elapsedTime) {
    if (status == TrainStatus::DEPARTED) {
        // Calculate distance traveled during elapsed time
        auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(elapsedTime).count();
        double distanceCovered = speed * (static_cast<double>(elapsedSeconds) / 3600.0); // Speed in km/h, time in hours

        distanceTraveled += distanceCovered;

        // You would need to calculate the total distance between origin and destination
        // For simplicity, let's assume a fixed total distance for now or calculate it based on the travelDuration and speed
        double totalDistance = std::chrono::duration_cast<std::chrono::seconds>(travelDuration).count() * (speed / 3600.0);

        if (distanceTraveled >= totalDistance) {
            status = TrainStatus::ARRIVED;
            currentStation = destination;
            distanceTraveled = totalDistance; // Ensure distance doesn't exceed total
            std::cout << "Train " << trainNumber << " has arrived at " << destination << std::endl;
        } else {
            // You could implement a more complex position tracking here,
            // e.g., calculating the percentage of the journey completed
            // For now, we just update the distance traveled.
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Train& train) {
    os << "Train Number: " << train.trainNumber
       << ", Capacity: " << train.capacity
       << ", Available Seats: " << train.getAvailableSeats()
       << ", Current Station: " << train.currentStation
       << ", Status: ";
    switch (train.status) {
        case TrainStatus::SCHEDULED: os << "Scheduled"; break;
        case TrainStatus::DEPARTED: os << "Departed"; break;
        case TrainStatus::ARRIVED: os << "Arrived"; break;
    }
    os << ", Origin: " << train.origin
       << ", Destination: " << train.destination
       << ", Distance Traveled: " << train.distanceTraveled << " km";
    // You might want to add departure time and travel duration printing
    return os;
}