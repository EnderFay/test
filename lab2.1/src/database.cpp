#include "../include/database.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <ctime>


// Station operations
void Database::addStation(const Station& station) {
    stations.push_back(station);
}

void Database::removeStation(int id) {
    stations.erase(std::remove_if(stations.begin(), stations.end(), 
        [id](const Station& s) { return s.getId() == id; }), stations.end());
}

void Database::editStation(int id, const std::string& name, const std::string& city) {
    for (auto& station : stations) {
        if (station.getId() == id) {
            station.setName(name);
            station.setCity(city);
            break;
        }
    }
}

const std::vector<Station>& Database::getAllStations() const {
    return stations;
}

Station* Database::findStation(int id) {
    auto it = std::find_if(stations.begin(), stations.end(), 
        [id](const Station& s) { return s.getId() == id; });
    return it != stations.end() ? &(*it) : nullptr;
}

const Station* Database::findStation(int id) const {
    auto it = std::find_if(stations.begin(), stations.end(), 
        [id](const Station& s) { return s.getId() == id; });
    return it != stations.end() ? &(*it) : nullptr;
}

// Route operations
void Database::addRoute(const Route& route) {
    routes.push_back(route);
}

void Database::removeRoute(int id) {
    routes.erase(std::remove_if(routes.begin(), routes.end(), 
        [id](const Route& r) { return r.getId() == id; }), routes.end());
}

void Database::editRoute(int id, const std::string& name, double distance) {
    for (auto& route : routes) {
        if (route.getId() == id) {
            route.setName(name);
            route.setDistance(distance);
            break;
        }
    }
}

const std::vector<Route>& Database::getAllRoutes() const {
    return routes;
}

Route* Database::findRoute(int id) {
    auto it = std::find_if(routes.begin(), routes.end(), 
        [id](const Route& r) { return r.getId() == id; });
    return it != routes.end() ? &(*it) : nullptr;
}

const Route* Database::findRoute(int id) const {
    auto it = std::find_if(routes.begin(), routes.end(), 
        [id](const Route& r) { return r.getId() == id; });
    return it != routes.end() ? &(*it) : nullptr;
}

// Train operations
void Database::addTrain(const Train& train) {
    trains.push_back(train);
}

void Database::removeTrain(int id) {
    trains.erase(std::remove_if(trains.begin(), trains.end(), 
        [id](const Train& t) { return t.getId() == id; }), trains.end());
}

void Database::editTrain(int id, const std::string& number, int totalSeats) {
    for (auto& train : trains) {
        if (train.getId() == id) {
            train.setNumber(number);
            train.setTotalSeats(totalSeats);
            break;
        }
    }
}

const std::vector<Train>& Database::getAllTrains() const {
    return trains;
}

Train* Database::findTrain(int id) {
    auto it = std::find_if(trains.begin(), trains.end(), 
        [id](const Train& t) { return t.getId() == id; });
    return it != trains.end() ? &(*it) : nullptr;
}

const Train* Database::findTrain(int id) const {
    auto it = std::find_if(trains.begin(), trains.end(), 
        [id](const Train& t) { return t.getId() == id; });
    return it != trains.end() ? &(*it) : nullptr;
}

void Database::updateTrainStatuses(const std::tm& currentTime) {
    for (auto& train : trains) {
        train.updateStatus(currentTime);
    }
}

// Ticket operations
void Database::addTicket(const Ticket& ticket) {
    tickets.push_back(ticket);
}

void Database::returnTicket(int id) {
    for (auto& ticket : tickets) {
        if (ticket.getId() == id) {
            ticket.returnTicket();
            // Return seats to the train
            if (Train* train = findTrain(ticket.getTrainId())) {
                train->cancelBooking(1);
            }
            break;
        }
    }
}

const std::vector<Ticket>& Database::getAllTickets() const {
    return tickets;
}

void Database::setCurrentTime(const std::tm& time) {
    currentTime = time;
}

// Helper functions
void Database::displayAllStations() const {
    for (const auto& station : stations) {
        station.displayInfo();
        std::cout << "-----------------\n";
    }
}

void Database::displayAllRoutes() const {
    for (const auto& route : routes) {
        route.displayInfo(stations);
        std::cout << "-----------------\n";
    }
}

void Database::displayAllTrains() const {
    for (const auto& train : trains) {
        if (const Route* route = this->findRoute(train.getRouteId())) {
            train.displayInfo(*route);
            std::cout << "-----------------\n";
        }
    }
}

void Database::displayAllTickets() const {
    for (const auto& ticket : tickets) {
        if (const Train* train = this->findTrain(ticket.getTrainId())) {
            ticket.displayInfo(*train);
            std::cout << "-----------------\n";
        }
    }
}

// Time simulation
void Database::simulateTimePassing(int days) {
    currentTime.tm_mday += days;
    std::mktime(&currentTime);
    updateTrainStatuses(currentTime);
}