#include<iostream>
#include <iomanip> 
#include <fstream>
#include<ctime>
#include<string>
#include <sstream>
#include "RusRails.h"
using namespace std;

int Train::getId() const {
    return id;
}

int Train::getLim() {
    return limit;
}

time_t Train::getStart() {
    return start;
}

void Train::setLim(int x) {
    limit = x;
}

int Train::getPath() {
    return path;
}

void Train::setId(int x) {
    id = x;
}

int Train::getCurStation() {
    return curSt;
}

void Train::changeCurStation(time_t curTime, vector<Route>* routes, int routesCount, vector<Station>* stations, vector<Ticket>* tickets, int* ticketCount) {
    Route* temp = nullptr;
    time_t pastTense = start;

    for (int i = 0; i < routesCount; i++) {
        if (routes->at(i).getId() == path) {
            temp = &(routes->at(i));
            break;
        }
    }

    if (temp == nullptr) {
        cout << "Что-то пошло не так" << endl;
        return;
    }

    if (direction) {
        for (int i = 0; i < temp->getStatinsCount() - 1; i++) {
            int StId = (temp->getStations())[i];
            int nextStId = (temp->getStations())[i + 1];
            Station* tempSt = nullptr;

            for (int j = 0; j < stations->size(); j++) {
                if (stations->at(j).getId() == StId) {
                    tempSt = &(stations->at(j));
                    break;
                }
            }

            int x;
            for (int j = 0; j < tempSt->getNumAdj(); j++) {
                if (tempSt->getAdjacentStations()[j] == nextStId) {
                    x = j;
                    break;
                }
            }

            if (pastTense + (tempSt->getDistances()[x]) * 3600 < curTime) {
                pastTense += (tempSt->getDistances()[x]) * 3600;
            }
            else {
                curSt = nextStId;
                return;
            }
        }
        direction = false;
    }

    if (!direction) {
        pastTense = start;
        for (int i = 0; i < temp->getStatinsCount() - 1; i++) {
            int StId = (temp->getStations())[i];
            int nextStId = (temp->getStations())[i + 1];
            Station* tempSt = nullptr;

            for (int j = 0; j < stations->size(); j++) {
                if (stations->at(j).getId() == StId) {
                    tempSt = &(stations->at(j));
                    break;
                }
            }

            int x;
            for (int j = 0; j < tempSt->getNumAdj(); j++) {
                if (tempSt->getAdjacentStations()[j] == nextStId) {
                    x = j;
                    break;
                }
            }
            pastTense += (tempSt->getDistances()[x]) * 3600;
        }

        for (int i = temp->getStatinsCount() - 1; i > 0; i--) {
            int StId = (temp->getStations())[i];
            int nextStId = (temp->getStations())[i - 1];
            Station* tempSt = nullptr;

            for (int j = 0; j < stations->size(); j++) {
                if (stations->at(j).getId() == StId) {
                    tempSt = &(stations->at(j));
                    break;
                }
            }

            int x;
            for (int j = 0; j < tempSt->getNumAdj(); j++) {
                if (tempSt->getAdjacentStations()[j] == nextStId) {
                    x = j;
                    break;
                }
            }

            if (pastTense + (tempSt->getDistances()[x]) * 3600 < curTime) {
                pastTense += (tempSt->getDistances()[x]) * 3600;
            }
            else {
                curSt = nextStId;
                return;
            }
        }
    }

    start = curTime + 86400;
    direction = true;
    vector<int> forRemove;

    for (int i = 0; i < *ticketCount; i++) {
        if (tickets->at(i).getTrain() == id) {
            forRemove.push_back(tickets->at(i).getId());
        }
    }

    for (int i = 0; i < forRemove.size(); i++) {
        for (int j = 0; j < *ticketCount; j++) {
            if (forRemove[i] == tickets->at(j).getId()) {
                tickets->erase(tickets->begin() + j);
                (*ticketCount)--;
                break;
            }
        }
    }
}

ostream& operator <<(ostream& out, const Train& r) {
    tm* normalTime = localtime(&r.start);
    out << r.id << " " << r.limit << " " << normalTime->tm_year + 1900 << " " << normalTime->tm_mon + 1 << " " << normalTime->tm_mday << " " << normalTime->tm_hour << " " << r.curSt << " " << r.path;
    return out;
}

istream& operator >>(istream& in, Train& r) {
    cout << "Введите лимит пассажиров: ";
    int limit;
    while (!(in >> r.limit).good() || r.limit < 1 || r.limit > 50) {
        cout << "Неправильное значение, введите значение от 1 до 50 " << endl;
    }

    return in;
}

bool Train::setStartTime(time_t globalTime) {
    string inputTime;
    cout << "Введите дату и время отправки (в формате YYYY-MM-DD HH): ";
    cin.get();
    getline(cin, inputTime);
    tm tm = {};
    istringstream times(inputTime);
    times >> get_time(&tm, "YYYY-MM-DD HH");

    if (times.fail()) {
        std::cerr << "Ошибка: неверный формат ввода." << std::endl;
        return 0;
    }

    time_t timeStart = mktime(&tm);

    if (timeStart == -1) {
        std::cerr << "Ошибка: не удалось преобразовать в time_t." << std::endl;
        return 0;
    }

    if (timeStart >= globalTime)
        start = timeStart;
    return 1;
}

bool Train::setPath(vector<Route>* routes, int routesCount) {
    for (int i = 0; i < routesCount; i++) {
        cout << routes->at(i) << endl;
    }
    cout << "Введите id маршрута" << endl;
    int stid;
    if (!(cin >> stid).good()) {
        cout << "Syntax error" << endl;
        return 0;
    }

    Route* temp = nullptr;
    for (int i = 0; i < routesCount; i++) {
        if (routes->at(i).getId() == stid) {
            temp = &(routes->at(i));
            break;
        }
    }

    temp->addTrain(id);
    curSt = temp->getStations()[0];
    path = stid;

    return 1;
}

Train* Train::getPasAndTime(vector<Ticket>* tickets, int ticketCount, vector<Station>* stations, int stationCount, vector<Route>* routes, int routesCount, int stationA, int stationB, time_t globalTime) {
    time_t pasTense = start;
    Route* tempRoute = nullptr;

    for (int i = 0; i < routesCount; i++) {
        if (routes->at(i).getId() == path) {
            tempRoute = &(routes->at(i));
            break;
        }
    }

    if (tempRoute == nullptr) {
        cout << "Ошибка" << endl;
        return NULL;
    }

    int a, b;
    for (int i = 0; i < tempRoute->getStatinsCount(); i++) {
        if (tempRoute->getStations()[i] == stationA) {
            a = i;
        }
        else if (tempRoute->getStations()[i] == stationB) {
            b = i;
        }
    }

    bool dir = true;
    if (a > b) dir = false;

    if (!dir) {
        for (int i = 0; i < tempRoute->getStatinsCount() - 1; i++) {
        }
    }
}
