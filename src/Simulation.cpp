#include "Simulation.h"
#include <iostream>

Simulation::Simulation(DatabaseManager& dbManager) : dbManager(dbManager) {
    currentTime = std::chrono::system_clock::now();
}

void Simulation::advanceTime(const std::chrono::system_clock::duration& duration) {
    currentTime += duration;
    std::cout << "Time advanced. Current time: ";
    std::time_t tt = std::chrono::system_clock::to_time_t(currentTime);
    std::tm tm_struct;
#ifdef _WIN32
    localtime_s(&tm_struct, &tt);
#else
    localtime_r(&tt, &tm_struct);
#endif
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm_struct);
    std::cout << buffer << std::endl;

    updateTrainStatuses();
}

void Simulation::updateTrainStatuses() {
    for (auto& train : dbManager.trains) {
        if (train.status == TrainStatus::SCHEDULED) {
            if (currentTime >= train.departureTime) {
                train.status = TrainStatus::DEPARTED;
                train.currentStation = "Between " + train.origin + " and " + train.destination;
                std::cout << "Train " << train.trainNumber << " has departed from " << train.origin << std::endl;
            }
        }

        if (train.status == TrainStatus::DEPARTED) {
            // Calculate elapsed time since departure
            auto elapsedTime = currentTime - train.departureTime;
            train.updatePosition(elapsedTime);
        }
    }
}