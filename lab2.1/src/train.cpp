#include "../include/train.h"
#include "../include/route.h"
#include <iostream>
#include <iomanip>
#include <ctime>

Train::Train(int id, const std::string& number, int routeId, int totalSeats, 
             const std::tm& depTime, const std::tm& arrTime)
    : id(id), number(number), routeId(routeId), totalSeats(totalSeats), 
      availableSeats(totalSeats), departureTime(depTime), arrivalTime(arrTime), 
      status(TrainStatus::WAITING) {}

int Train::getId() const { return id; }
std::string Train::getNumber() const { return number; }
int Train::getRouteId() const { return routeId; }
int Train::getTotalSeats() const { return totalSeats; }
int Train::getAvailableSeats() const { return availableSeats; }
std::tm Train::getDepartureTime() const { return departureTime; }
std::tm Train::getArrivalTime() const { return arrivalTime; }
TrainStatus Train::getStatus() const { return status; }

std::string Train::getStatusString() const {
    switch (status) {
        case TrainStatus::WAITING: return "Waiting";
        case TrainStatus::ON_WAY: return "On the way";
        case TrainStatus::ARRIVED: return "Arrived";
        case TrainStatus::CANCELLED: return "Cancelled";
        default: return "Unknown";
    }
}

void Train::setNumber(const std::string& newNumber) { number = newNumber; }
void Train::setTotalSeats(int newTotalSeats) { 
    totalSeats = newTotalSeats; 
    if (availableSeats > totalSeats) {
        availableSeats = totalSeats;
    }
}
void Train::setDepartureTime(const std::tm& newTime) { departureTime = newTime; }
void Train::setArrivalTime(const std::tm& newTime) { arrivalTime = newTime; }
void Train::setStatus(TrainStatus newStatus) { status = newStatus; }

bool Train::bookSeats(int count) {
    if (availableSeats >= count) {
        availableSeats -= count;
        return true;
    }
    return false;
}

bool Train::cancelBooking(int count) {
    if (availableSeats + count <= totalSeats) {
        availableSeats += count;
        return true;
    }
    return false;
}

void Train::displayInfo(const Route& route) const {
    std::cout << "Train ID: " << id << "\n";
    std::cout << "Number: " << number << "\n";
    std::cout << "Route: " << route.getName() << "\n";
    std::cout << "Status: " << getStatusString() << "\n";
    std::cout << "Seats: " << availableSeats << "/" << totalSeats << " available\n";
    
    std::cout << "Departure: " << std::put_time(&departureTime, "%Y-%m-%d %H:%M") << "\n";
    std::cout << "Arrival: " << std::put_time(&arrivalTime, "%Y-%m-%d %H:%M") << "\n";
}

void Train::updateStatus(const std::tm& currentTime) {
    if (status == TrainStatus::CANCELLED) return;
    
    std::time_t now = std::mktime(const_cast<std::tm*>(&currentTime));
    std::time_t dep = std::mktime(&departureTime);
    std::time_t arr = std::mktime(&arrivalTime);
    
    if (now < dep) {
        status = TrainStatus::WAITING;
    } else if (now >= dep && now < arr) {
        status = TrainStatus::ON_WAY;
    } else {
        status = TrainStatus::ARRIVED;
    }
}