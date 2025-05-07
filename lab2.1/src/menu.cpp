#include "../include/menu.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <limits>
#include <algorithm>

Menu::Menu() {
    std::time_t t = std::time(nullptr);
    currentTime = *std::localtime(&t);
    
    // Initialize database time
    std::time_t now = std::time(nullptr);
    std::tm now_tm = *std::localtime(&now);
    db.setCurrentTime(now_tm);

    db.simulateTimePassing(0);

    // Add some initial data for demonstration
    db.addStation(Station(1, "Moscow", "Moscow"));
    db.addStation(Station(2, "Saint Petersburg", "Saint Petersburg"));
    db.addStation(Station(3, "Novosibirsk", "Novosibirsk"));
    
    std::vector<int> intermediate = {3};
    db.addRoute(Route(1, "Moscow - Saint Petersburg", 1, 2, {}, 650));
    db.addRoute(Route(2, "Trans-Siberian", 1, 3, {}, 3300));
    
    std::tm depTime = currentTime;
    depTime.tm_hour = 8;
    depTime.tm_min = 0;
    std::tm arrTime = depTime;
    arrTime.tm_hour += 4; // 4 hours trip
    
    db.addTrain(Train(1, "SAPSAN-001", 1, 500, depTime, arrTime));
    
    depTime.tm_mday += 1;
    arrTime = depTime;
    arrTime.tm_hour += 12; // 2 days trip
    db.addTrain(Train(2, "ROSSIYA-002", 2, 300, depTime, arrTime));
}

void Menu::printCurrentTime() const {
    std::cout << "Current system time: " << std::put_time(&currentTime, "%Y-%m-%d %H:%M") << "\n";
}

void Menu::run() {
    while (true) {
        std::cout << "\n=== RZD Railway Management System ===\n";
        printCurrentTime();
        std::cout << "1. Admin login\n";
        std::cout << "2. User menu\n";
        std::cout << "3. Simulate time passing\n";
        std::cout << "4. Exit\n";
        std::cout << "Select option: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: adminMenu(); break;
            case 2: userMenu(); break;
            case 3: {
                std::cout << "Enter number of days to simulate: ";
                int days;
                std::cin >> days;
                db.simulateTimePassing(days);
                std::cout << "Time advanced by " << days << " days.\n";
                break;
            }
            case 4: return;
            default: std::cout << "Invalid choice. Try again.\n";
        }
    }
}

void Menu::adminMenu() {
    while (true) {
        std::cout << "\n=== Admin Menu ===\n";
        std::cout << "1. Manage stations\n";
        std::cout << "2. Manage routes\n";
        std::cout << "3. Manage trains\n";
        std::cout << "4. View tickets\n";
        std::cout << "5. Back to main menu\n";
        std::cout << "Select option: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: manageStations(); break;
            case 2: manageRoutes(); break;
            case 3: manageTrains(); break;
            case 4: viewTickets(); break;
            case 5: return;
            default: std::cout << "Invalid choice. Try again.\n";
        }
    }
}

void Menu::manageStations() {
    while (true) {
        std::cout << "\n=== Station Management ===\n";
        std::cout << "1. Add station\n";
        std::cout << "2. Remove station\n";
        std::cout << "3. Edit station\n";
        std::cout << "4. View all stations\n";
        std::cout << "5. Back\n";
        std::cout << "Select option: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (choice == 5) break;
        
        switch (choice) {
            case 1: {
                std::string name, city;
                std::cout << "Enter station name: ";
                std::getline(std::cin, name);
                std::cout << "Enter city: ";
                std::getline(std::cin, city);
                
                int newId = 1;
                if (!db.getAllStations().empty()) {
                    newId = db.getAllStations().back().getId() + 1;
                }
                db.addStation(Station(newId, name, city));
                std::cout << "Station added successfully.\n";
                break;
            }
            case 2: {
                db.displayAllStations();
                std::cout << "Enter station ID to remove: ";
                int id;
                std::cin >> id;
                db.removeStation(id);
                std::cout << "Station removed if it existed.\n";
                break;
            }
            case 3: {
                db.displayAllStations();
                std::cout << "Enter station ID to edit: ";
                int id;
                std::cin >> id;
                std::cin.ignore();
                
                std::string name, city;
                std::cout << "Enter new name: ";
                std::getline(std::cin, name);
                std::cout << "Enter new city: ";
                std::getline(std::cin, city);
                
                db.editStation(id, name, city);
                std::cout << "Station updated if it existed.\n";
                break;
            }
            case 4:
                db.displayAllStations();
                break;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    }
}

void Menu::manageRoutes() {
    while (true) {
        std::cout << "\n=== Route Management ===\n";
        std::cout << "1. Add route\n";
        std::cout << "2. Remove route\n";
        std::cout << "3. Edit route\n";
        std::cout << "4. View all routes\n";
        std::cout << "5. Back\n";
        std::cout << "Select option: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (choice == 5) break;
        
        switch (choice) {
            case 1: {
                db.displayAllStations();
                
                std::string name;
                std::cout << "Enter route name: ";
                std::getline(std::cin, name);
                
                int depId, arrId;
                std::cout << "Enter departure station ID: ";
                std::cin >> depId;
                std::cout << "Enter arrival station ID: ";
                std::cin >> arrId;
                
                std::vector<int> intermediate;
                std::cout << "Enter intermediate station IDs (space separated, -1 to finish): ";
                int id;
                while (std::cin >> id && id != -1) {
                    intermediate.push_back(id);
                }
                std::cin.clear();
                
                double distance;
                std::cout << "Enter distance in km: ";
                std::cin >> distance;
                
                int newId = 1;
                if (!db.getAllRoutes().empty()) {
                    newId = db.getAllRoutes().back().getId() + 1;
                }
                db.addRoute(Route(newId, name, depId, arrId, intermediate, distance));
                std::cout << "Route added successfully.\n";
                break;
            }
            case 2: {
                db.displayAllRoutes();
                std::cout << "Enter route ID to remove: ";
                int id;
                std::cin >> id;
                db.removeRoute(id);
                std::cout << "Route removed if it existed.\n";
                break;
            }
            case 3: {
                db.displayAllRoutes();
                std::cout << "Enter route ID to edit: ";
                int id;
                std::cin >> id;
                std::cin.ignore();
                
                std::string name;
                std::cout << "Enter new name: ";
                std::getline(std::cin, name);
                
                double distance;
                std::cout << "Enter new distance: ";
                std::cin >> distance;
                
                db.editRoute(id, name, distance);
                std::cout << "Route updated if it existed.\n";
                break;
            }
            case 4:
                db.displayAllRoutes();
                break;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    }
}

void Menu::manageTrains() {
    while (true) {
        std::cout << "\n=== Train Management ===\n";
        std::cout << "1. Add train\n";
        std::cout << "2. Remove train\n";
        std::cout << "3. Edit train\n";
        std::cout << "4. View all trains\n";
        std::cout << "5. Back\n";
        std::cout << "Select option: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (choice == 5) break;
        
        switch (choice) {
            case 1: {
                db.displayAllRoutes();
                
                std::string number;
                std::cout << "Enter train number: ";
                std::getline(std::cin, number);
                
                int routeId, totalSeats;
                std::cout << "Enter route ID: ";
                std::cin >> routeId;
                std::cout << "Enter total seats: ";
                std::cin >> totalSeats;
                
                std::tm depTime = getDateTimeInput("Enter departure time");
                std::tm arrTime = getDateTimeInput("Enter arrival time");
                
                int newId = 1;
                if (!db.getAllTrains().empty()) {
                    newId = db.getAllTrains().back().getId() + 1;
                }
                db.addTrain(Train(newId, number, routeId, totalSeats, depTime, arrTime));
                std::cout << "Train added successfully.\n";
                break;
            }
            case 2: {
                db.displayAllTrains();
                std::cout << "Enter train ID to remove: ";
                int id;
                std::cin >> id;
                db.removeTrain(id);
                std::cout << "Train removed if it existed.\n";
                break;
            }
            case 3: {
                db.displayAllTrains();
                std::cout << "Enter train ID to edit: ";
                int id;
                std::cin >> id;
                std::cin.ignore();
                
                std::string number;
                std::cout << "Enter new train number: ";
                std::getline(std::cin, number);
                
                int totalSeats;
                std::cout << "Enter new total seats: ";
                std::cin >> totalSeats;
                
                db.editTrain(id, number, totalSeats);
                std::cout << "Train updated if it existed.\n";
                break;
            }
            case 4:
                db.displayAllTrains();
                break;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    }
}

void Menu::viewTickets() {
    db.displayAllTickets();
}

void Menu::userMenu() {
    while (true) {
        std::cout << "\n=== User Menu ===\n";
        std::cout << "1. Buy ticket\n";
        std::cout << "2. Return ticket\n";
        std::cout << "3. View schedule\n";
        std::cout << "4. Back to main menu\n";
        std::cout << "Select option: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: buyTicket(); break;
            case 2: returnTicket(); break;
            case 3: viewSchedule(); break;
            case 4: return;
            default: std::cout << "Invalid choice. Try again.\n";
        }
    }
}

void Menu::buyTicket() {
    db.displayAllStations();
    
    int fromId, toId;
    std::cout << "Enter departure station ID: ";
    std::cin >> fromId;
    std::cout << "Enter arrival station ID: ";
    std::cin >> toId;
    
    // Find trains that go from fromId to toId
    std::vector<Train*> availableTrains;
    for (auto& train : const_cast<std::vector<Train>&>(db.getAllTrains())) {
        const Route* route = db.findRoute(train.getRouteId());
        if (!route) continue;
        
        const std::vector<int>& intermediate = route->getIntermediateStations();
        
        if ((route->getDepartureStationId() == fromId && 
             route->getArrivalStationId() == toId) ||
            (std::find(intermediate.begin(), intermediate.end(), fromId) != intermediate.end() &&
             std::find(intermediate.begin(), intermediate.end(), toId) != intermediate.end())) {
            
            train.updateStatus(currentTime);
            if (train.getStatus() == TrainStatus::WAITING && train.getAvailableSeats() > 0) {
                availableTrains.push_back(&train);
            }
        }
    }
    
    if (availableTrains.empty()) {
        std::cout << "No available trains for this route.\n";
        return;
    }
    
    std::cout << "\nAvailable trains:\n";
    for (size_t i = 0; i < availableTrains.size(); ++i) {
        if (const Route* route = db.findRoute(availableTrains[i]->getRouteId())) {
            std::cout << i+1 << ". ";
            availableTrains[i]->displayInfo(*route);
            std::cout << "-----------------\n";
        }
    }
    
    std::cout << "Select train (1-" << availableTrains.size() << "): ";
    size_t selection;
    std::cin >> selection;
    std::cin.ignore();
    
    if (selection < 1 || selection > availableTrains.size()) {
        std::cout << "Invalid selection.\n";
        return;
    }
    
    Train* selectedTrain = availableTrains[selection-1];
    
    std::string name, passport, contact;
    std::cout << "Enter passenger name: ";
    std::getline(std::cin, name);
    std::cout << "Enter passport number: ";
    std::getline(std::cin, passport);
    std::cout << "Enter contact info: ";
    std::getline(std::cin, contact);
    
    Passenger passenger(name, passport, contact);
    
    // Simple price calculation based on distance
    double price = 0;
    if (const Route* route = db.findRoute(selectedTrain->getRouteId())) {
        price = route->getDistance() * 2.5; // 2.5 RUB per km
    }
    
    int newId = 1;
    if (!db.getAllTickets().empty()) {
        newId = db.getAllTickets().back().getId() + 1;
    }
    
    if (selectedTrain->bookSeats(1)) {
        db.addTicket(Ticket(newId, selectedTrain->getId(), passenger, currentTime, price));
        std::cout << "Ticket purchased successfully for " << price << " RUB\n";
    } else {
        std::cout << "Failed to book ticket. No available seats.\n";
    }
}

void Menu::returnTicket() {
    std::cout << "Enter your passport number: ";
    std::string passport;
    std::getline(std::cin, passport);
    
    std::vector<const Ticket*> userTickets;
    for (const auto& ticket : db.getAllTickets()) {
        if (ticket.getPassenger().getPassportNumber() == passport && !ticket.getIsReturned()) {
            userTickets.push_back(&ticket);
        }
    }
    
    if (userTickets.empty()) {
        std::cout << "No active tickets found for this passport.\n";
        return;
    }
    
    std::cout << "\nYour tickets:\n";
    for (size_t i = 0; i < userTickets.size(); ++i) {
        if (const Train* train = db.findTrain(userTickets[i]->getTrainId())) {
            std::cout << i+1 << ". ";
            userTickets[i]->displayInfo(*train);
            std::cout << "-----------------\n";
        }
    }
    
    std::cout << "Select ticket to return (1-" << userTickets.size() << "): ";
    size_t selection;
    std::cin >> selection;
    std::cin.ignore();
    
    if (selection < 1 || selection > userTickets.size()) {
        std::cout << "Invalid selection.\n";
        return;
    }
    
    db.returnTicket(userTickets[selection-1]->getId());
    std::cout << "Ticket returned successfully.\n";
}

void Menu::viewSchedule() {
    db.displayAllTrains();
}

std::tm Menu::getDateTimeInput(const std::string& prompt) const {
    std::tm time = currentTime;
    
    std::cout << prompt << " (YYYY MM DD HH MM): ";
    std::cin >> time.tm_year >> time.tm_mon >> time.tm_mday >> time.tm_hour >> time.tm_min;
    time.tm_year -= 1900;
    time.tm_mon -= 1;
    
    return time;
}