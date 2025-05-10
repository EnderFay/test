#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <ctime>
#include "station.h"
#include "route.h"
#include "train.h"
#include "ticket.h"

class Database {
    private:
        std::vector<Station> stations;
        std::vector<Route> routes;
        std::vector<Train> trains;
        std::vector<Ticket> tickets;
        std::tm currentTime;
        
    public:
        // Station operations
        void addStation(const Station& station);
        void removeStation(int id);
        void editStation(int id, const std::string& name, const std::string& city);
        const std::vector<Station>& getAllStations() const;
        Station* findStation(int id);
        const Station* findStation(int id) const;
        
        // Route operations
        void addRoute(const Route& route);
        void removeRoute(int id);
        void editRoute(int id, const std::string& name, double distance);
        const std::vector<Route>& getAllRoutes() const;
        Route* findRoute(int id);
        const Route* findRoute(int id) const;
        
        // Train operations
        void addTrain(const Train& train);
        void removeTrain(int id);
        void editTrain(int id, const std::string& number, int totalSeats);
        const std::vector<Train>& getAllTrains() const;
        Train* findTrain(int id);
        const Train* findTrain(int id) const;
        void updateTrainStatuses(const std::tm& currentTime);
        
        // Ticket operations
        void addTicket(const Ticket& ticket);
        void returnTicket(int id);
        const std::vector<Ticket>& getAllTickets() const;
        void setCurrentTime(const std::tm& time);
        
        // Helper functions
        void displayAllStations() const;
        void displayAllRoutes() const;
        void displayAllTrains() const;
        void displayAllTickets() const;
        
        // Time simulation
        void simulateTimePassing(int days);

	//Data persistetence
	bool saveData(const std::string& filename) const;
	bool loadData(const std::string& filename);

private:
	// Helper functions for saving/loading specific objects
    bool saveStations(std::ofstream& file) const;
    bool loadStations(std::ifstream& file);

    bool saveRoutes(std::ofstream& file) const;
    bool loadRoutes(std::ifstream& file);

    bool saveTrains(std::ofstream& file) const;
    bool loadTrains(std::ifstream& file);

    bool saveTickets(std::ofstream& file) const;
    bool loadTickets(std::ifstream& file);

};

#endif // DATABASE_H
