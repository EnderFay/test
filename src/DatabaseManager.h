#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <vector>
#include <list>
#include "Direction.h"
#include "Train.h"
#include "Ticket.h"
#include <fstream>
#include <sstream>

class DatabaseManager {
public:
    std::vector<Direction> directions;
    std::list<Train> trains; // Using std::list for trains
    std::vector<Ticket> tickets; // Using std::vector for tickets

    // Functions for managing Directions
    void addDirection(const Direction& direction);
    void removeDirection(const std::string& origin, const std::string& destination);
    void viewDirections() const;
    Direction* findDirection(const std::string& origin, const std::string& destination);

    // Functions for managing Trains
    void addTrain(const Train& train);
    void removeTrain(int trainNumber);
    void viewTrains() const;
    Train* findTrain(int trainNumber);
    const Train* findTrain(int trainNumber) const;
    void viewTrainsByDirection(const std::string& origin, const std::string& destination) const;

    // Functions for managing Tickets
    void addTicket(const Ticket& ticket);
    void removeTicket(int ticketId);
    void viewTickets() const;
    Ticket* findTicket(int ticketId);
    void viewTicketsByTrain(int trainNumber) const;

    // Data persistence (saving and loading) - basic examples
    void saveDatabases(const std::string& directionsFile, const std::string& trainsFile, const std::string& ticketsFile) const;
    void loadDatabases(const std::string& directionsFile, const std::string& trainsFile, const std::string& ticketsFile);

private:
    // Helper functions for saving/loading (optional, can be in .cpp)
    void saveDirections(const std::string& filename) const;
    void loadDirections(const std::string& filename);
    void saveTrains(const std::string& filename) const;
    void loadTrains(const std::string& filename);
    void saveTickets(const std::string& filename) const;
    void loadTickets(const std::string& filename);
};

#endif // DATABASE_MANAGER_H