#include "Direction.h"

Direction::Direction(const std::string& origin, const std::string& destination)
    : origin(origin), destination(destination) {}

std::ostream& operator<<(std::ostream& os, const Direction& direction) {
    os << "Origin: " << direction.origin << ", Destination: " << direction.destination;
    return os;
}