#ifndef TICKET_H
#define TICKET_H

#include <string>
#include <iostream>

class Ticket {
public:
    int ticketId; // Unique identifier for the ticket
    std::string passengerName;
    int trainNumber; // To link with Train
    int seatNumber; // Seat number on the train

    Ticket(int ticketId, const std::string& passengerName, int trainNumber, int seatNumber);

    friend std::ostream& operator<<(std::ostream& os, const Ticket& ticket);
};

#endif // TICKET_H