#include "../include/ticket.h"
#include "../include/train.h"
#include <iostream>
#include <iomanip>

Ticket::Ticket(int id, int trainId, const Passenger& passenger, 
               const std::tm& bookingTime, double price)
    : id(id), trainId(trainId), passenger(passenger), 
      bookingTime(bookingTime), price(price), isReturned(false) {}

int Ticket::getId() const { return id; }
int Ticket::getTrainId() const { return trainId; }
Passenger Ticket::getPassenger() const { return passenger; }
std::tm Ticket::getBookingTime() const { return bookingTime; }
double Ticket::getPrice() const { return price; }
bool Ticket::getIsReturned() const { return isReturned; }

void Ticket::returnTicket() { isReturned = true; }

void Ticket::displayInfo(const Train& train) const {
    std::cout << "Ticket ID: " << id << "\n";
    std::cout << "Train: " << train.getNumber() << "\n";
    passenger.displayInfo();
    std::cout << "Booking time: " << std::put_time(&bookingTime, "%Y-%m-%d %H:%M") << "\n";
    std::cout << "Price: " << price << " RUB\n";
    std::cout << "Status: " << (isReturned ? "Returned" : "Active") << "\n";
}