#include "Ticket.h"

Ticket::Ticket(int ticketId, const std::string& passengerName, int trainNumber, int seatNumber)
    : ticketId(ticketId), passengerName(passengerName), trainNumber(trainNumber), seatNumber(seatNumber) {}

std::ostream& operator<<(std::ostream& os, const Ticket& ticket) {
    os << "Ticket ID: " << ticket.ticketId
       << ", Passenger: " << ticket.passengerName
       << ", Train Number: " << ticket.trainNumber
       << ", Seat: " << ticket.seatNumber;
    return os;
}