#ifndef TRAIN_H
#define TRAIN_H

#include <string>
#include <ctime>
#include "route.h"

enum class TrainStatus {
    WAITING,    // ожидает отправления
    ON_WAY,     // в пути
    ARRIVED,    // прибыл
    CANCELLED   // отменен
};

class Train {
private:
    int id;
    std::string number;
    int routeId;
    int totalSeats;
    int availableSeats;
    std::tm departureTime;
    std::tm arrivalTime;
    TrainStatus status;
    
public:
    Train(int id, const std::string& number, int routeId, int totalSeats, 
          const std::tm& depTime, const std::tm& arrTime);
    
    int getId() const;
    std::string getNumber() const;
    int getRouteId() const;
    int getTotalSeats() const;
    int getAvailableSeats() const;
    std::tm getDepartureTime() const;
    std::tm getArrivalTime() const;
    TrainStatus getStatus() const;
    std::string getStatusString() const;
    
    void setNumber(const std::string& newNumber);
    void setTotalSeats(int newTotalSeats);
    void setDepartureTime(const std::tm& newTime);
    void setArrivalTime(const std::tm& newTime);
    void setStatus(TrainStatus newStatus);
    
    bool bookSeats(int count);
    bool cancelBooking(int count);
    
    void displayInfo(const Route& route) const;
    void updateStatus(const std::tm& currentTime);
};

#endif // TRAIN_H