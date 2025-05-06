#include <iostream>
#include <limits> // For numeric_limits
#include <chrono>
#include <iomanip> // For std::get_time

#include "DatabaseManager.h"
#include "Simulation.h"
#include "Direction.h"
#include "Train.h"
#include "Ticket.h"

// Function to get integer input with validation
int getIntInput() {
    int value;
    while (!(std::cin >> value)) {
        std::cout << "Invalid input. Please enter an integer: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Consume the rest of the line
    return value;
}

// Function to get double input with validation
double getDoubleInput() {
    double value;
    while (!(std::cin >> value)) {
        std::cout << "Invalid input. Please enter a number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Consume the rest of the line
    return value;
}

// Function to get a line of string input
std::string getStringInput() {
    std::string value;
    std::getline(std::cin, value);
    return value;
}

// Function to get time_point input
std::chrono::system_clock::time_point getTimePointInput() {
    std::cout << "Enter date and time (YYYY-MM-DD HH:MM:SS): ";
    std::string timeString;
    std::getline(std::cin, timeString);
    std::tm tm_struct = {};
    std::stringstream ss(timeString);
    ss >> std::get_time(&tm_struct, "%Y-%m-%d %H:%M:%S");
    if (ss.fail()) {
        std::cerr << "Invalid time format. Using current time." << std::endl;
        return std::chrono::system_clock::now();
    }
    std::time_t tt = std::mktime(&tm_struct);
    return std::chrono::system_clock::from_time_t(tt);
}

// Function to get duration input
std::chrono::system_clock::duration getDurationInput() {
    std::cout << "Enter travel duration in seconds: ";
    long long seconds = getIntInput();
    return std::chrono::seconds(seconds);
}


void displayMenu() {
    std::cout << "\n--- RZD Simulation Menu ---" << std::endl;
    std::cout << "1. Manage Directions" << std::endl;
    std::cout << "2. Manage Trains" << std::endl;
    std::cout << "3. Manage Tickets" << std::endl;
    std::cout << "4. View Train Status by Direction" << std::endl;
    std::cout << "5. View Specific Train Status" << std::endl;
    std::cout << "6. Advance Time" << std::endl;
    std::cout << "7. Save Databases" << std::endl;
    std::cout << "8. Load Databases" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

void manageDirections(DatabaseManager& dbManager) {
    int choice;
    do {
        std::cout << "\n--- Manage Directions ---" << std::endl;
        std::cout << "1. Add Direction" << std::endl;
        std::cout << "2. Remove Direction" << std::endl;
        std::cout << "3. View All Directions" << std::endl;
        std::cout << "0. Back to Main Menu" << std::endl;
        std::cout << "Enter your choice: ";
        choice = getIntInput();

        switch (choice) {
            case 1: {
                std::cout << "Enter origin: ";
                std::string origin = getStringInput();
                std::cout << "Enter destination: ";
                std::string destination = getStringInput();
                dbManager.addDirection(Direction(origin, destination));
                break;
            }
            case 2: {
                std::cout << "Enter origin of the direction to remove: ";
                std::string origin = getStringInput();
                std::cout << "Enter destination of the direction to remove: ";
                std::string destination = getStringInput();
                dbManager.removeDirection(origin, destination);
                break;
            }
            case 3: {
                dbManager.viewDirections();
                break;
            }
            case 0:
                std::cout << "Returning to main menu." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 0);
}

void manageTrains(DatabaseManager& dbManager) {
    int choice;
    do {
        std::cout << "\n--- Manage Trains ---" << std::endl;
        std::cout << "1. Add Train" << std::endl;
        std::cout << "2. Remove Train" << std::endl;
        std::cout << "3. View All Trains" << std::endl;
        std::cout << "0. Back to Main Menu" << std::endl;
        std::cout << "Enter your choice: ";
        choice = getIntInput();

        switch (choice) {
            case 1: {
                std::cout << "Enter train number: ";
                int trainNumber = getIntInput();
                std::cout << "Enter capacity: ";
                int capacity = getIntInput();
                std::cout << "Enter origin station: ";
                std::string origin = getStringInput();
                std::cout << "Enter destination station: ";
                std::string destination = getStringInput();
                std::chrono::system_clock::time_point departureTime = getTimePointInput();
                std::chrono::system_clock::duration travelDuration = getDurationInput();
                std::cout << "Enter train speed (km/h): ";
                double speed = getDoubleInput();

                dbManager.addTrain(Train(trainNumber, capacity, origin, destination, departureTime, travelDuration, speed));
                break;
            }
            case 2: {
                std::cout << "Enter train number to remove: ";
                int trainNumber = getIntInput();
                dbManager.removeTrain(trainNumber);
                break;
            }
            case 3: {
                dbManager.viewTrains();
                break;
            }
            case 0:
                std::cout << "Returning to main menu." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 0);
}

void manageTickets(DatabaseManager& dbManager) {
    int choice;
    do {
        std::cout << "\n--- Manage Tickets ---" << std::endl;
        std::cout << "1. Buy Ticket" << std::endl;
        std::cout << "2. Return Ticket" << std::endl;
        std::cout << "3. View All Tickets" << std::endl;
        std::cout << "4. View Tickets by Train" << std::endl;
        std::cout << "0. Back to Main Menu" << std::endl;
        std::cout << "Enter your choice: ";
        choice = getIntInput();

        switch (choice) {
            case 1: {
                std::cout << "Enter passenger name: ";
                std::string passengerName = getStringInput();
                std::cout << "Enter train number: ";
                int trainNumber = getIntInput();

                Train* train = dbManager.findTrain(trainNumber);
                if (train) {
                    if (train->status == TrainStatus::SCHEDULED) {
                        if (train->getAvailableSeats() > 0) {
                            // Simple ticket ID generation (can be improved)
                            int ticketId = dbManager.tickets.empty() ? 1 : dbManager.tickets.back().ticketId + 1;
                            // Simple seat assignment (can be improved)
                            int seatNumber = train->capacity - train->getAvailableSeats() + 1;
                            Ticket newTicket(ticketId, passengerName, trainNumber, seatNumber);
                            if (train->buyTicket(newTicket)) {
                                dbManager.addTicket(newTicket);
                                std::cout << "Ticket purchased successfully!" << std::endl;
                            } else {
                                std::cout << "Failed to purchase ticket. No available seats." << std::endl;
                            }
                        } else {
                            std::cout << "Train " << trainNumber << " is full." << std::endl;
                        }
                    } else {
                        std::cout << "Cannot buy ticket for train " << trainNumber << ". It has already departed or arrived." << std::endl;
                    }
                } else {
                    std::cout << "Train " << trainNumber << " not found." << std::endl;
                }
                break;
            }
            case 2: {
                std::cout << "Enter ticket ID to return: ";
                int ticketId = getIntInput();
                Ticket* ticket = dbManager.findTicket(ticketId);
                if (ticket) {
                    Train* train = dbManager.findTrain(ticket->trainNumber);
                    if (train) {
                        if (train->status == TrainStatus::SCHEDULED) {
                            if (train->returnTicket(ticketId)) {
                                dbManager.removeTicket(ticketId);
                                std::cout << "Ticket returned successfully!" << std::endl;
                            } else {
                                std::cout << "Failed to return ticket for train " << train->trainNumber << std::endl;
                            }
                        } else {
                            std::cout << "Cannot return ticket. Train " << train->trainNumber << " has already departed or arrived." << std::endl;
                        }
                    } else {
                        std::cout << "Associated train not found for ticket " << ticketId << std::endl;
                    }
                } else {
                    std::cout << "Ticket " << ticketId << " not found." << std::endl;
                }
                break;
            }
            case 3: {
                dbManager.viewTickets();
                break;
            }
            case 4: {
                std::cout << "Enter train number to view tickets: ";
                int trainNumber = getIntInput();
                dbManager.viewTicketsByTrain(trainNumber);
                break;
            }
            case 0:
                std::cout << "Returning to main menu." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 0);
}

void viewTrainStatusByDirection(const DatabaseManager& dbManager) {
    std::cout << "Enter origin station: ";
    std::string origin = getStringInput();
    std::cout << "Enter destination station: ";
    std::string destination = getStringInput();
    dbManager.viewTrainsByDirection(origin, destination);
}

void viewSpecificTrainStatus(const DatabaseManager& dbManager) {
    std::cout << "Enter train number: ";
    int trainNumber = getIntInput();
    const Train* train = dbManager.findTrain(trainNumber);
    if (train) {
        std::cout << *train << std::endl;
        // Calculate time remaining if scheduled or departed
        if (train->status == TrainStatus::SCHEDULED) {
            auto timeToDeparture = train->departureTime - std::chrono::system_clock::now();
            auto hours = std::chrono::duration_cast<std::chrono::hours>(timeToDeparture).count();
            auto minutes = std::chrono::duration_cast<std::chrono::minutes>(timeToDeparture % std::chrono::hours(1)).count();
            std::cout << "Time until departure: " << hours << " hours, " << minutes << " minutes" << std::endl;
        } else if (train->status == TrainStatus::DEPARTED) {
            // Need to calculate total distance and remaining distance
            double totalDistance = std::chrono::duration_cast<std::chrono::seconds>(train->travelDuration).count() * (train->speed / 3600.0);
            double remainingDistance = totalDistance - train->distanceTraveled;
            if (train->speed > 0) {
                double timeRemainingHours = remainingDistance / train->speed;
                auto timeRemainingDuration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::duration<double, std::ratio<3600>>(timeRemainingHours));
                auto hours = std::chrono::duration_cast<std::chrono::hours>(timeRemainingDuration).count();
                auto minutes = std::chrono::duration_cast<std::chrono::minutes>(timeRemainingDuration % std::chrono::hours(1)).count();
                 std::cout << "Estimated time to destination (" << train->destination << "): " << hours << " hours, " << minutes << " minutes" << std::endl;
            } else {
                 std::cout << "Cannot calculate time to destination (speed is 0)." << std::endl;
            }
        }
    } else {
        std::cout << "Train " << trainNumber << " not found." << std::endl;
    }
}

void advanceTime(Simulation& simulation) {
    std::cout << "Enter time to advance (in seconds): ";
    long long seconds = getIntInput();
    simulation.advanceTime(std::chrono::seconds(seconds));
}

int main() {
    DatabaseManager dbManager;
    Simulation simulation(dbManager);

    // Load data on startup
    dbManager.loadDatabases("directions.csv", "trains.csv", "tickets.csv");

    int choice;
    do {
        displayMenu();
        choice = getIntInput();

        switch (choice) {
            case 1:
                manageDirections(dbManager);
                break;
            case 2:
                manageTrains(dbManager);
                break;
            case 3:
                manageTickets(dbManager);
                break;
            case 4:
                viewTrainStatusByDirection(dbManager);
                break;
            case 5:
                viewSpecificTrainStatus(dbManager);
                break;
            case 6:
                advanceTime(simulation);
                break;
            case 7:
                dbManager.saveDatabases("directions.csv", "trains.csv", "tickets.csv");
                break;
            case 8:
                dbManager.loadDatabases("directions.csv", "trains.csv", "tickets.csv");
                break;
            case 0:
                std::cout << "Exiting program. Saving data..." << std::endl;
                dbManager.saveDatabases("directions.csv", "trains.csv", "tickets.csv");
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 0);

    return 0;
}