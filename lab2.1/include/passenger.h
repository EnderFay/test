#ifndef PASSENGER_H
#define PASSENGER_H

#include <string>

class Passenger {
private:
    std::string name;
    std::string passportNumber;
    std::string contactInfo;
    
public:
    Passenger(const std::string& name, const std::string& passport, 
              const std::string& contact);
    
    std::string getName() const;
    std::string getPassportNumber() const;
    std::string getContactInfo() const;
    
    void setName(const std::string& newName);
    void setPassportNumber(const std::string& newPassport);
    void setContactInfo(const std::string& newContact);
    
    void displayInfo() const;
};

#endif // PASSENGER_H