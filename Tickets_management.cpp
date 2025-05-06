#include <algorithm>

#include "RusRails.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

namespace rjd {
    const char* filename = "tickets.txt";

    void write(vector<Ticket>* tickets, int n) {
        ofstream out(filename);
        if (!out.is_open()) {
            cout << "Не удалось открыть файл для записи." << endl;
            return;
        }
        out.write(reinterpret_cast<const char*>(&n), sizeof(int));
        out.write(reinterpret_cast<const char*>(&(tickets->at(0))), sizeof(Ticket) * n);
        out.close();
    }

    void print(vector<Ticket>* tickets, int ticketCount) {
        cout << ticketCount << endl;
        for (int i = 0; i < ticketCount; i++) {
            cout << tickets->at(i) << endl;
        }
    }

    void read(std::vector<Ticket>* tickets, int* ticketCount) {
        ifstream in(filename);
        if (!in.is_open()) {
            cout << "Не удалось открыть файл для чтения." << endl;
            return;
        }
        in.read(reinterpret_cast<char*>(ticketCount), sizeof(int));
        tickets->resize(*ticketCount);
        in.read(reinterpret_cast<char*>(&(tickets->at(0))), sizeof(Ticket) * *ticketCount);
        in.close();
    }

    void create(vector<Ticket>* tickets, int* ticketCount) {
        cout << "Введите количество билетов: ";
        if (!(cin >> *ticketCount).good()) {
            cout << "Ошибка синтаксиса" << endl;
            return;
        }
        tickets->resize(*ticketCount);
        for (int i = 0; i < *ticketCount; ++i) {
            cout << "Введите данные для билета " << i + 1 << ":\n";
            Ticket temp;
            if (!(cin >> temp).good()) {
                cout << "Ошибка синтаксиса" << endl;
                return;
            }
        }
        write(tickets, *ticketCount);
    }

    void add(vector<Ticket>* tickets, int* ticketCount, Ticket newTicket) {
        if (*ticketCount == 0) {
            newTicket.setId(0);
        }
        else {
            newTicket.setId(tickets->end()->getId() + 1);
        }
        tickets->push_back(newTicket);
        (*ticketCount)++;
        write(tickets, *ticketCount);
    }

    void remove(vector<Ticket>* tickets, int* ticketCount) {
        int ticketId;
        cout << "Введите номер билета для удаления: " << endl;
        if (!(cin >> ticketId).good()) {
            cout << "Ошибка синтаксиса" << endl;
            return;
        }
        auto it = remove_if(tickets->begin(), tickets->end(), [ticketId](Ticket& t) { return t.getId() == ticketId; });
        if (it != tickets->end()) {
            tickets->erase(it, tickets->end());
            --(*ticketCount);
            write(tickets, *ticketCount);
            cout << "Билет удален." << endl;
        }
        else {
            cout << "Билет с таким номером не найден." << endl;
        }
    }

    void edit(std::vector<Ticket>* tickets, int ticketCount) {
        int ticketId;
        cout << "Введите номер билета для редактирования: ";
        if (!(cin >> ticketId).good()) {
            cout << "Ошибка синтаксиса" << endl;
            return;
        }
        auto it = find_if(tickets->begin(), tickets->end(), [ticketId](Ticket& t) { return t.getId() == ticketId; });
        if (it != tickets->end()) {
            cout << "Введите новые данные для билета:" << endl;
            if (!(cin >> *it).good()) {
                cout << "Ошибка синтаксиса" << endl;
                return;
            }
            write(tickets, ticketCount);
            cout << "Билет отредактирован." << endl;
        }
        else {
            cout << "Билет с таким номером не найден." << endl;
        }
    }
}
