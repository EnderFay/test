#ifndef STATION_H
#define STATION_H

#include <string>
#include <vector>

class Station {
private:
    int id;
    std::string name;
    std::string city;
    
public:
    Station(int id, const std::string& name, const std::string& city);
    
    int getId() const;
    std::string getName() const;
    std::string getCity() const;
    
    void setName(const std::string& newName);
    void setCity(const std::string& newCity);
    
    void displayInfo() const;
};

#endif // STATION_H