#include "DatabaseManager.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <chrono>

// Helper function to convert time_point to a string (for saving)
std::string timePointToString(const std::chrono::system_clock::time_point& tp) {
    std::time_t tt = std::chrono::system_clock::to_time_t(tp);
    std::tm tm_struct;
#ifdef _WIN32
    localtime_s(&tm_struct, &tt);
#else
    localtime_r(&tt, &tm_struct);
#endif
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm_struct);
    return buffer;
}

// Helper function to convert a string to time_point (for loading)
std::chrono::system_clock::time_point stringToTimePoint(const std::string& s) {
    std::tm tm_struct = {};
    std::stringstream ss(s);
    ss >> std::get_time(&tm_struct, "%Y-%m-%d %H:%M:%S");
    if (ss.fail()) {
        throw std::runtime_error{"failed to parse time string"};
    }
    std::time_t tt = std::mktime(&tm_struct);
    return std::chrono::system_clock::from_time_t(tt);
}

// Functions for managing Directions
void DatabaseManager::addDirection(const Direction& direction) {
    directions.push_back(direction);
    std::cout << "Direction added: " << direction << std::endl;
}

void DatabaseManager::removeDirection(const std::string& origin, const std::string& destination) {
    auto it = directions.begin();
    while (it != directions.end()) {
        if (it->origin == origin && it->destination == destination) {
            it = directions.erase(it);
            std::cout << "Direction removed: From " << origin << " to " << destination << std::endl;
            return;
        } else {
            ++it;
        }
    }
    std::cout << "Direction not found: From " << origin << " to " << destination << std::endl;
}

void DatabaseManager::viewDirections() const {
    if (directions.empty()) {
        std::cout << "No directions in the database." << std::endl;
        return;
    }
    std::cout << "--- Directions ---" << std::endl;
    for (const auto& dir : directions) {
        std::cout << dir << std::endl;
    }
    std::cout << "------------------" << std::endl;
}

Direction* DatabaseManager::findDirection(const std::string& origin, const std::string& destination) {
    for (auto& dir : directions) {
        if (dir.origin == origin && dir.destination == destination) {
            return &dir;
        }
    }
    return nullptr;
}

// Functions for managing Trains
void DatabaseManager::addTrain(const Train& train) {
    trains.push_back(train);
    std::cout << "Train added: " << train.trainNumber << std::endl;
}

void DatabaseManager::removeTrain(int trainNumber) {
    auto it = trains.begin();
    while (it != trains.end()) {
        if (it->trainNumber == trainNumber) {
            it = trains.erase(it);
            std::cout << "Train removed: " << trainNumber << std::endl;
            return;
        } else {
            ++it;
        }
    }
    std::cout << "Train not found: " << trainNumber << std::endl;
}

void DatabaseManager::viewTrains() const {
    if (trains.empty()) {
        std::cout << "No trains in the database." << std::endl;
        return;
    }
    std::cout << "--- Trains ---" << std::endl;
    for (const auto& train : trains) {
        std::cout << train << std::endl;
    }
    std::cout << "------------------" << std::endl;
}

Train* DatabaseManager::findTrain(int trainNumber) {
    for (auto it = trains.begin(); it != trains.end(); ++it) {
        if (it->getTrainNumber() == trainNumber) {
            return &(*it);
        }
    }
    return nullptr;
}

const Train* DatabaseManager::findTrain(int trainNumber) const{
    for (auto& train : trains) {
        if (train.trainNumber == trainNumber) {
            return &train;
        }
    }
    return nullptr;
}

void DatabaseManager::viewTrainsByDirection(const std::string& origin, const std::string& destination) const {
    bool found = false;
    std::cout << "--- Trains for Direction: " << origin << " to " << destination << " ---" << std::endl;
    for (const auto& train : trains) {
        if (train.origin == origin && train.destination == destination) {
            std::cout << train << std::endl;
            found = true;
        }
    }
    if (!found) {
        std::cout << "No trains found for this direction." << std::endl;
    }
    std::cout << "-------------------------------------------------" << std::endl;
}

// Functions for managing Tickets
void DatabaseManager::addTicket(const Ticket& ticket) {
    tickets.push_back(ticket);
    std::cout << "Ticket added: " << ticket.ticketId << std::endl;
}

void DatabaseManager::removeTicket(int ticketId) {
    auto it = tickets.begin();
    while (it != tickets.end()) {
        if (it->ticketId == ticketId) {
            it = tickets.erase(it);
            std::cout << "Ticket removed: " << ticketId << std::endl;
            return;
        } else {
            ++it;
        }
    }
    std::cout << "Ticket not found: " << ticketId << std::endl;
}

void DatabaseManager::viewTickets() const {
    if (tickets.empty()) {
        std::cout << "No tickets in the database." << std::endl;
        return;
    }
    std::cout << "--- Tickets ---" << std::endl;
    for (const auto& ticket : tickets) {
        std::cout << ticket << std::endl;
    }
    std::cout << "---------------" << std::endl;
}

Ticket* DatabaseManager::findTicket(int ticketId) {
    for (auto& ticket : tickets) {
        if (ticket.ticketId == ticketId) {
            return &ticket;
        }
    }
    return nullptr;
}

void DatabaseManager::viewTicketsByTrain(int trainNumber) const {
    bool found = false;
    std::cout << "--- Tickets for Train: " << trainNumber << " ---" << std::endl;
    for (const auto& ticket : tickets) {
        if (ticket.trainNumber == trainNumber) {
            std::cout << ticket << std::endl;
            found = true;
        }
    }
    if (!found) {
        std::cout << "No tickets found for this train." << std::endl;
    }
    std::cout << "---------------------------------" << std::endl;
}

// Data persistence
void DatabaseManager::saveDatabases(const std::string& directionsFile, const std::string& trainsFile, const std::string& ticketsFile) const {
    saveDirections(directionsFile);
    saveTrains(trainsFile);
    saveTickets(ticketsFile);
    std::cout << "Databases saved successfully." << std::endl;
}

void DatabaseManager::loadDatabases(const std::string& directionsFile, const std::string& trainsFile, const std::string& ticketsFile) {
    loadDirections(directionsFile);
    loadTrains(trainsFile);
    loadTickets(ticketsFile);
    std::cout << "Databases loaded successfully." << std::endl;
}

void DatabaseManager::saveDirections(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error opening directions file for saving: " << filename << std::endl;
        return;
    }
    for (const auto& dir : directions) {
        outFile << dir.origin << "," << dir.destination << std::endl;
    }
    outFile.close();
}

void DatabaseManager::loadDirections(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Error opening directions file for loading: " << filename << std::endl;
        return;
    }
    directions.clear(); // Clear existing data
    std::string line;
    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::string origin, destination;
        if (std::getline(ss, origin, ',') && std::getline(ss, destination, ',')) {
            directions.emplace_back(origin, destination);
        }
    }
    inFile.close();
}

void DatabaseManager::saveTrains(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error opening trains file for saving: " << filename << std::endl;
        return;
    }
    for (const auto& train : trains) {
        outFile << train.trainNumber << ","
                << train.capacity << ","
                << train.currentStation << ","
                << timePointToString(train.departureTime) << ","
                << std::chrono::duration_cast<std::chrono::seconds>(train.travelDuration).count() << "," // Save duration in seconds
                << static_cast<int>(train.status) << ","
                << train.origin << ","
                << train.destination << ","
                << train.speed << ","
                << train.distanceTraveled << std::endl;
    }
    outFile.close();
}

void DatabaseManager::loadTrains(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Error opening trains file for loading: " << filename << std::endl;
        return;
    }
    trains.clear(); // Clear existing data
    std::string line;
    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::string segment;
        std::vector<std::string> segments;
        while (std::getline(ss, segment, ',')) {
            segments.push_back(segment);
        }

        if (segments.size() == 10) {
            try {
                int trainNumber = std::stoi(segments[0]);
                int capacity = std::stoi(segments[1]);
                std::string currentStation = segments[2];
                std::chrono::system_clock::time_point departureTime = stringToTimePoint(segments[3]);
                auto travelDuration = std::chrono::seconds(std::stoi(segments[4]));
                TrainStatus status = static_cast<TrainStatus>(std::stoi(segments[5]));
                std::string origin = segments[6];
                std::string destination = segments[7];
                double speed = std::stod(segments[8]);
                double distanceTraveled = std::stod(segments[9]);

                Train loadedTrain(trainNumber, capacity, origin, destination, departureTime, travelDuration, speed);
                loadedTrain.currentStation = currentStation;
                loadedTrain.status = status;
                loadedTrain.distanceTraveled = distanceTraveled;
                trains.push_back(loadedTrain);

            } catch (const std::exception& e) {
                std::cerr << "Error parsing train data: " << e.what() << " in line: " << line << std::endl;
            }
        }
    }
    inFile.close();
}

void DatabaseManager::saveTickets(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error opening tickets file for saving: " << filename << std::endl;
        return;
    }
    for (const auto& ticket : tickets) {
        outFile << ticket.ticketId << ","
                << ticket.passengerName << ","
                << ticket.trainNumber << ","
                << ticket.seatNumber << std::endl;
    }
    outFile.close();
}

void DatabaseManager::loadTickets(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Error opening tickets file for loading: " << filename << std::endl;
        return;
    }
    tickets.clear(); // Clear existing data
    std::string line;
    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::string segment;
        std::vector<std::string> segments;
        while (std::getline(ss, segment, ',')) {
            segments.push_back(segment);
        }

        if (segments.size() == 4) {
            try {
                int ticketId = std::stoi(segments[0]);
                std::string passengerName = segments[1];
                int trainNumber = std::stoi(segments[2]);
                int seatNumber = std::stoi(segments[3]);
                tickets.emplace_back(ticketId, passengerName, trainNumber, seatNumber);
            } catch (const std::exception& e) {
                std::cerr << "Error parsing ticket data: " << e.what() << " in line: " << line << std::endl;
            }
        }
    }
    inFile.close();
}