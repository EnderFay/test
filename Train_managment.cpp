#include "RusRails.h"
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

namespace rjd {

    void write(vector<Train>* trains, int n) {
        ofstream out("trains.txt");
        if (!out.is_open()) {
            cout << "Не удалось открыть файл" << endl;
            return;
        }
        out.write(reinterpret_cast<char*>(&n), sizeof(int));
        out.write(reinterpret_cast<char*>(&(trains->at(0))), sizeof(Train) * n);
        out.close();
    }

    void create(vector<Train>* trains, int* n, time_t globalTime, vector<Route>* routes, int routesCount) {
        cout << "Введите количество поездов (n): ";
        if (!(cin >> (*n)).good()) {
            cout << "Синтаксическая ошибка" << endl;
            return;
        }
        trains->resize(*n);
        for (int i = 0; i < *n; i++) {
            Train temp;
            if (!(cin >> temp).good()) {
                cout << "Что-то пошло не так" << endl;
                return;
            }
            temp.setId(i);
            if (!temp.setStartTime(globalTime)) {
                cout << "Что-то пошло не так" << endl;
                return;
            }
            if (!temp.setPath(routes, routesCount)) {
                cout << "Что-то пошло не так" << endl;
                return;
            }
            trains->at(i) = temp;
        }
        write(trains, *n);
        write(routes, routesCount);
    }

    void printTrains(vector<Train>* trains, int n) {
        cout << n << endl;
        for (int i = 0; i < n; i++) {
            cout << trains->at(i) << endl;
        }
    }

    void read(vector<Train>* trains, int* n) {
        ifstream in("trains.txt");
        if (!in.is_open()) {
            cout << "Файл не был открыт корректно" << endl;
            return;
        }
        in.read(reinterpret_cast<char*>(n), sizeof(int));
        trains->resize(*n);
        in.read(reinterpret_cast<char*>(&(trains->at(0))), sizeof(Train) * (*n));
        printTrains(trains, *n);
    }

    void add(vector<Train>* trains, int* n, time_t globalTime, vector<Route>* routes, int routesCount) {
        Train temp;
        if (!(cin >> temp).good()) {
            cout << "Что-то пошло не так" << endl;
            return;
        }
        temp.setId(*n == 0 ? 0 : trains->back().getId() + 1);
        if (!temp.setStartTime(globalTime)) {
            cout << "Что-то пошло не так" << endl;
            return;
        }
        if (!temp.setPath(routes, routesCount)) {
            cout << "Что-то пошло не так" << endl;
            return;
        }
        (*n)++;
        trains->push_back(temp);
        write(trains, *n);
        write(routes, routesCount);
    }

    void remove(vector<Train>* trains, int* n, vector<Route>* routes, int routesCount) {
        printTrains(trains, *n);
        cout << "Выберите поезд для удаления: ";
        int id;
        if (!(cin >> id).good()) {
            cout << "Синтаксическая ошибка" << endl;
            return;
        }

        int x = -1;
        for (int i = 0; i < *n; i++) {
            if (trains->at(i).getId() == id) {
                x = i;
                break;
            }
        }

        if (x == -1) {
            cout << "Поезд не найден" << endl;
            return;
        }

        for (int i = 0; i < routesCount; i++) {
            if (routes->at(i).getId() == trains->at(x).getPath()) {
                routes->at(i).removeTrain(id);
                break;
            }
        }

        trains->erase(trains->begin() + x);
        (*n)--;
        write(trains, *n);
        write(routes, routesCount);
    }

    void edit(vector<Train>* trains, int n) {
        printTrains(trains, n);
        cout << "Выберите поезд для редактирования: ";
        int id;
        if (!(cin >> id).good()) {
            cout << "Синтаксическая ошибка" << endl;
            return;
        }

        Train* temp = nullptr;
        for (int i = 0; i < n; i++) {
            if (trains->at(i).getId() == id) {
                temp = &(trains->at(i));
                break;
            }
        }

        if (temp == nullptr) {
            cout << "Ошибка ввода: поезд не найден" << endl;
            return;
        }

        cout << *temp << endl;
        cout << "Что редактировать:" << endl;
        cout << "1 - Лимит пассажиров" << endl;
        cout << "Вариант выбора = ";
        int key;
        if (!(cin >> key).good()) {
            cout << "Синтаксическая ошибка" << endl;
            return;
        }

        if (key == 1) {
            while (true) {
                cout << "Введите лимит пассажиров от 1 до 50: ";
                int x;
                if (!(cin >> x).good()) {
                    cout << "Введите число из интервала" << endl;
                    continue;
                }
                if (x < 1 || x > 50) {
                    cout << "Введите число из интервала" << endl;
                }
                else {
                    temp->setLim(x);
                    break;
                }
            }
        }

        write(trains, n);
    }

}
