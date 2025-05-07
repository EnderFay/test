#include "../include/passenger.h"
#include <iostream>

Passenger::Passenger(const std::string& name, const std::string& passport, 
                     const std::string& contact)
    : name(name), passportNumber(passport), contactInfo(contact) {}

std::string Passenger::getName() const { return name; }
std::string Passenger::getPassportNumber() const { return passportNumber; }
std::string Passenger::getContactInfo() const { return contactInfo; }

void Passenger::setName(const std::string& newName) { name = newName; }
void Passenger::setPassportNumber(const std::string& newPassport) { passportNumber = newPassport; }
void Passenger::setContactInfo(const std::string& newContact) { contactInfo = newContact; }

void Passenger::displayInfo() const {
    std::cout << "Passenger: " << name << "\n";
    std::cout << "Passport: " << passportNumber << "\n";
    std::cout << "Contact: " << contactInfo << "\n";
}