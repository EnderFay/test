#include "../include/station.h"
#include <iostream>

Station::Station(int id, const std::string& name, const std::string& city)
    : id(id), name(name), city(city) {}

int Station::getId() const { return id; }
std::string Station::getName() const { return name; }
std::string Station::getCity() const { return city; }

void Station::setName(const std::string& newName) { name = newName; }
void Station::setCity(const std::string& newCity) { city = newCity; }

void Station::displayInfo() const {
    std::cout << "Station ID: " << id << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "City: " << city << "\n";
}