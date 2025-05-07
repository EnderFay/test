#include "../include/route.h"
#include "../include/station.h"
#include <iostream>
#include <algorithm>

Route::Route(int id, const std::string& name, int depStationId, int arrStationId, 
             const std::vector<int>& intermediate, double distance)
    : id(id), name(name), departureStationId(depStationId), 
      arrivalStationId(arrStationId), intermediateStations(intermediate), 
      distance(distance) {}

int Route::getId() const { return id; }
std::string Route::getName() const { return name; }
int Route::getDepartureStationId() const { return departureStationId; }
int Route::getArrivalStationId() const { return arrivalStationId; }
std::vector<int> Route::getIntermediateStations() const { return intermediateStations; }
double Route::getDistance() const { return distance; }

void Route::setName(const std::string& newName) { name = newName; }
void Route::setDistance(double newDistance) { distance = newDistance; }

void Route::displayInfo(const std::vector<Station>& stations) const {
    std::cout << "Route ID: " << id << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Distance: " << distance << " km\n";
    
    auto depStation = std::find_if(stations.begin(), stations.end(), 
        [this](const Station& s) { return s.getId() == departureStationId; });
    auto arrStation = std::find_if(stations.begin(), stations.end(), 
        [this](const Station& s) { return s.getId() == arrivalStationId; });
    
    if (depStation != stations.end()) {
        std::cout << "Departure: " << depStation->getName() << "\n";
    }
    
    if (!intermediateStations.empty()) {
        std::cout << "Intermediate stations: ";
        for (int stationId : intermediateStations) {
            auto it = std::find_if(stations.begin(), stations.end(), 
                [stationId](const Station& s) { return s.getId() == stationId; });
            if (it != stations.end()) {
                std::cout << it->getName() << " ";
            }
        }
        std::cout << "\n";
    }
    
    if (arrStation != stations.end()) {
        std::cout << "Arrival: " << arrStation->getName() << "\n";
    }
}

std::string Route::getRouteDescription(const std::vector<Station>& stations) const {
    std::string description;
    
    auto depStation = std::find_if(stations.begin(), stations.end(), 
        [this](const Station& s) { return s.getId() == departureStationId; });
    auto arrStation = std::find_if(stations.begin(), stations.end(), 
        [this](const Station& s) { return s.getId() == arrivalStationId; });
    
    if (depStation != stations.end()) {
        description += depStation->getName();
    }
    
    description += " -> ";
    
    for (int stationId : intermediateStations) {
        auto it = std::find_if(stations.begin(), stations.end(), 
            [stationId](const Station& s) { return s.getId() == stationId; });
        if (it != stations.end()) {
            description += it->getName() + " -> ";
        }
    }
    
    if (arrStation != stations.end()) {
        description += arrStation->getName();
    }
    
    return description;
}