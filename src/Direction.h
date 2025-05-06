#ifndef DIRECTION_H
#define DIRECTION_H

#include <string>
#include <vector>
#include <iostream>

class Direction {
public:
    std::string origin;
    std::string destination;
    // You might want to add a list of trains for this direction here

    Direction(const std::string& origin, const std::string& destination);

    // Overload the output operator for easy printing
    friend std::ostream& operator<<(std::ostream& os, const Direction& direction);
};

#endif // DIRECTION_H