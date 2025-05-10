#include "database.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <limits> // Добавлено

// Helper function to trim whitespace from strings
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return "";
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

// Station operations
void Database::addStation(const Station& station) {
    stations.push_back(station);
}

void Database::removeStation(int id) {
    stations.erase(std::remove_if(stations.begin(), stations.end(),
        [id](const Station& s) { return s.getId() == id; }), stations.end());
}

void Database::editStation(int id, const std::string& name, const std::string& city) {
    for (auto& station : stations) {
        if (station.getId() == id) {
            station.setName(name);
            station.setCity(city);
            break;
        }
    }
}

const std::vector<Station>& Database::getAllStations() const {
    return stations;
}

Station* Database::findStation(int id) {
    auto it = std::find_if(stations.begin(), stations.end(),
        [id](const Station& s) { return s.getId() == id; });
    return it != stations.end() ? &(*it) : nullptr;
}

const Station* Database::findStation(int id) const {
    auto it = std::find_if(stations.begin(), stations.end(),
        [id](const Station& s) { return s.getId() == id; });
    return it != stations.end() ? &(*it) : nullptr;
}

// Route operations
void Database::addRoute(const Route& route) {
    routes.push_back(route);
}

void Database::removeRoute(int id) {
    routes.erase(std::remove_if(routes.begin(), routes.end(),
        [id](const Route& r) { return r.getId() == id; }), routes.end());
}

void Database::editRoute(int id, const std::string& name, double distance) {
    for (auto& route : routes) {
        if (route.getId() == id) {
            route.setName(name);
            route.setDistance(distance);
            break;
        }
    }
}

const std::vector<Route>& Database::getAllRoutes() const {
    return routes;
}

Route* Database::findRoute(int id) {
    auto it = std::find_if(routes.begin(), routes.end(),
        [id](const Route& r) { return r.getId() == id; });
    return it != routes.end() ? &(*it) : nullptr;
}

const Route* Database::findRoute(int id) const {
    auto it = std::find_if(routes.begin(), routes.end(),
        [id](const Route& r) { return r.getId() == id; });
    return it != routes.end() ? &(*it) : nullptr;
}

// Train operations
void Database::addTrain(const Train& train) {
    trains.push_back(train);
}

void Database::removeTrain(int id) {
    trains.erase(std::remove_if(trains.begin(), trains.end(),
        [id](const Train& t) { return t.getId() == id; }), trains.end());
}

void Database::editTrain(int id, const std::string& number, int totalSeats) {
    for (auto& train : trains) {
        if (train.getId() == id) {
            train.setNumber(number);
            train.setTotalSeats(totalSeats);
            break;
        }
    }
}

const std::vector<Train>& Database::getAllTrains() const {
    return trains;
}

Train* Database::findTrain(int id) {
    auto it = std::find_if(trains.begin(), trains.end(),
        [id](const Train& t) { return t.getId() == id; });
    return it != trains.end() ? &(*it) : nullptr;
}

const Train* Database::findTrain(int id) const {
    auto it = std::find_if(trains.begin(), trains.end(),
        [id](const Train& t) { return t.getId() == id; });
    return it != trains.end() ? &(*it) : nullptr;
}

void Database::updateTrainStatuses(const std::tm& currentTime) {
    for (auto& train : trains) {
        train.updateStatus(currentTime);
    }
}

// Ticket operations
void Database::addTicket(const Ticket& ticket) {
    tickets.push_back(ticket);
}

void Database::returnTicket(int id) {
    for (auto& ticket : tickets) {
        if (ticket.getId() == id) {
            ticket.returnTicket();
            // Return seats to the train
            if (Train* train = findTrain(ticket.getTrainId())) {
                train->cancelBooking(1);
            }
            break;
        }
    }
}

const std::vector<Ticket>& Database::getAllTickets() const {
    return tickets;
}

void Database::setCurrentTime(const std::tm& time) {
    currentTime = time;
}

// Helper functions
void Database::displayAllStations() const {
    for (const auto& station : stations) {
        station.displayInfo();
        std::cout << "-----------------\n";
    }
}

void Database::displayAllRoutes() const {
    for (const auto& route : routes) {
        route.displayInfo(stations);
        std::cout << "-----------------\n";
    }
}

void Database::displayAllTrains() const {
    for (const auto& train : trains) {
        if (const Route* route = this->findRoute(train.getRouteId())) {
            train.displayInfo(*route);
            std::cout << "-----------------\n";
        }
    }
}

void Database::displayAllTickets() const {
    for (const auto& ticket : tickets) {
        if (const Train* train = this->findTrain(ticket.getTrainId())) {
            ticket.displayInfo(*train);
            std::cout << "-----------------\n";
        }
    }
}

// Time simulation
void Database::simulateTimePassing(int days) {
    currentTime.tm_mday += days;
    std::mktime(&currentTime);
    updateTrainStatuses(currentTime);
}

// ----------------------- Data Persistence -----------------------

bool Database::saveData(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file for saving: " << filename << std::endl;
        return false;
    }

    if (!saveStations(file) || !saveRoutes(file) || !saveTrains(file) || !saveTickets(file)) {
        std::cerr << "Error saving data to file." << std::endl;
        file.close();
        return false;
    }

    file.close();
    std::cout << "Data saved to " << filename << std::endl;
    return true;
}

bool Database::loadData(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file for loading: " << filename << std::endl;
        return false;
    }

    if (!loadStations(file) || !loadRoutes(file) || !loadTrains(file) || !loadTickets(file)) {
        std::cerr << "Error loading data from file." << std::endl;
        file.close();
        return false;
    }

    file.close();
    std::cout << "Data loaded from " << filename << std::endl;
    return true;
}

// ----------------------- Station Persistence -----------------------

bool Database::saveStations(std::ofstream& file) const {
    file << "STATIONS " << stations.size() << std::endl;
    for (const auto& station : stations) {
        file << station.getId() << "\n"
             << station.getName() << "\n"
             << station.getCity() << std::endl;
    }
    return true;
}

bool Database::loadStations(std::ifstream& file) {
    std::string header;
    int count;

    file >> header >> count;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // consume the newline

    if (header != "STATIONS") {
        std::cerr << "Error: Expected STATIONS header, got " << header << std::endl;
        return false;
    }

    for (int i = 0; i < count; ++i) {
        int id;
        std::string name, city;

        file >> id;
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(file, name);
        std::getline(file, city);

        name = trim(name);
        city = trim(city);

        if (file.fail()) {
            std::cerr << "Error reading station data from file." << std::endl;
            return false;
        }

        stations.emplace_back(id, name, city);
    }
    return true;
}


// ----------------------- Route Persistence -----------------------

bool Database::saveRoutes(std::ofstream& file) const {
    file << "ROUTES " << routes.size() << std::endl;
    for (const auto& route : routes) {
        file << route.getId() << "\n"
             << route.getName() << "\n"
             << route.getDepartureStationId() << "\n"
             << route.getArrivalStationId() << "\n"
             << route.getDistance() << "\n";

        // Save intermediate stations
        const auto& intermediate = route.getIntermediateStations();
        file << intermediate.size() << std::endl;
        for (int stationId : intermediate) {
            file << stationId << std::endl;
        }
    }
    return true;
}

bool Database::loadRoutes(std::ifstream& file) {
    std::string header;
    int count;

    file >> header >> count;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (header != "ROUTES") {
        std::cerr << "Error: Expected ROUTES header, got " << header << std::endl;
        return false;
    }

    for (int i = 0; i < count; ++i) {
        int id, depId, arrId;
        std::string name;
        double distance;
        int numIntermediate;

        file >> id;
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(file, name);
        file >> depId >> arrId >> distance >> numIntermediate;
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::vector<int> intermediate(numIntermediate);
        for (int j = 0; j < numIntermediate; ++j) {
            file >> intermediate[j];
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        name = trim(name);

        if (file.fail()) {
            std::cerr << "Error reading route data from file." << std::endl;
            return false;
        }

        routes.emplace_back(id, name, depId, arrId, intermediate, distance);
    }
    return true;
}


// ----------------------- Train Persistence -----------------------

bool Database::saveTrains(std::ofstream& file) const {
   file << "TRAINS " << trains.size() << std::endl;
    for (const auto& train : trains) {
        file << train.getId() << "\n"
             << train.getNumber() << "\n"
             << train.getRouteId() << "\n"
             << train.getTotalSeats() << "\n"
             << train.getAvailableSeats() << "\n";

        // Save departure and arrival times using mktime
        std::tm depTimeCopy = train.getDepartureTime(); // Create a copy
        std::time_t depTimeT = std::mktime(&depTimeCopy);
        std::tm arrTimeCopy = train.getArrivalTime();   // Create a copy
        std::time_t arrTimeT = std::mktime(&arrTimeCopy);
        file << depTimeT << "\n" << arrTimeT << "\n";

        file << static_cast<int>(train.getStatus()) << std::endl;  // Save status as int
    }
    return true;
}

bool Database::loadTrains(std::ifstream& file) {
    std::string header;
    int count;

    file >> header >> count;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (header != "TRAINS") {
        std::cerr << "Error: Expected TRAINS header, got " << header << std::endl;
        return false;
    }

    for (int i = 0; i < count; ++i) {
        int id, routeId, totalSeats, availableSeats, statusInt;
        std::string number;
        std::time_t depTimeT, arrTimeT;

        file >> id;
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(file, number);
        file >> routeId >> totalSeats >> availableSeats >> depTimeT >> arrTimeT >> statusInt;
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::tm depTime = *std::localtime(&depTimeT);
        std::tm arrTime = *std::localtime(&arrTimeT);
        TrainStatus status = static_cast<TrainStatus>(statusInt);

        number = trim(number);

        if (file.fail()) {
            std::cerr << "Error reading train data from file." << std::endl;
            return false;
        }

        trains.emplace_back(id, number, routeId, totalSeats, depTime, arrTime);
        trains.back().setStatus(status); // Correct way to set status after object creation
    }
    return true;
}


// ----------------------- Ticket Persistence -----------------------

bool Database::saveTickets(std::ofstream& file) const {
    file << "TICKETS " << tickets.size() << std::endl;
    for (const auto& ticket : tickets) {
        file << ticket.getId() << "\n"
             << ticket.getTrainId() << "\n"
             << ticket.getPassenger().getName() << "\n"
             << ticket.getPassenger().getPassportNumber() << "\n"
             << ticket.getPassenger().getContactInfo() << "\n";

        // Save booking time using mktime
        std::tm bookingTimeCopy = ticket.getBookingTime(); // Create a copy
        std::time_t bookingTimeT = std::mktime(&bookingTimeCopy);

        file << bookingTimeT << "\n" << ticket.getPrice() << "\n" << ticket.getIsReturned() << std::endl;
    }
    return true;
}

bool Database::loadTickets(std::ifstream& file) {
    std::string header;
    int count;

    file >> header >> count;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (header != "TICKETS") {
        std::cerr << "Error: Expected TICKETS header, got " << header << std::endl;
        return false;
    }

    for (int i = 0; i < count; ++i) {
        int id, trainId;
        std::string passengerName, passportNumber, contactInfo;
        std::time_t bookingTimeT;
        double price;
        bool isReturned;

        file >> id;
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        file >> trainId;
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(file, passengerName);
        std::getline(file, passportNumber);
        std::getline(file, contactInfo);
        file >> bookingTimeT >> price >> isReturned;
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        passengerName = trim(passengerName);
        passportNumber = trim(passportNumber);
        contactInfo = trim(contactInfo);

        if (file.fail()) {
            std::cerr << "Error reading ticket data from file." << std::endl;
            return false;
        }

        std::tm bookingTime = *std::localtime(&bookingTimeT);
        Passenger passenger(passengerName, passportNumber, contactInfo);
        tickets.emplace_back(id, trainId, passenger, bookingTime, price);
        tickets.back().returnTicket(); // Correct way to set after object creation
    }
    return true;
}
