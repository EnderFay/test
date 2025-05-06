#ifndef SIMULATION_H
#define SIMULATION_H

#include "DatabaseManager.h"
#include <chrono>

class Simulation {
public:
    DatabaseManager& dbManager;
    std::chrono::system_clock::time_point currentTime;

    Simulation(DatabaseManager& dbManager);

    void advanceTime(const std::chrono::system_clock::duration& duration);

private:
    void updateTrainStatuses();
};

#endif // SIMULATION_H