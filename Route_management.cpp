#include "RusRails.h"
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

namespace rjd {
	void write(vector<Route>* routes, int n) {
		ofstream out;
		out.open("routes.txt");
		if (!out.is_open()) {
			cout << "Не удается открыть файл" << endl;
			return;
		}
		out.write(reinterpret_cast<char*>(&n), sizeof(int));
		out.write(reinterpret_cast<char*>(&(routes->at(0))), sizeof(Route) * n);
		out.close();
	}

	void create(vector<Route>* routes, int* n, vector<Station>* stations, int stationCount) {
		
		setlocale(LC_ALL, "Rus");
		cout << "n=";
		if (!(cin >> (*n)).good()) {
			cout << "Ошибка синтаксиса" << endl;
			return;
		}

		routes->resize((*n));
		for (int i = 0; i < *n; i++) {
			Route temp;
			
			int newStCount;
			cout << "Введите количество станций" << endl;
			if (!(cin >> newStCount).good()) {
				cout << "Ошибка синтаксиса" << endl;
				return;
			}
			if (newStCount > 20) {
				cout << "Ограничение = 20" << endl;
				return;
			}
			int* newSt = new int[newStCount];
			cout << "Введите первый id" << endl;
			if (!(cin >> newSt[0]).good()) {
				cout << "Ошибка синтаксиса" << endl;
				return;
			}
			int x = -1;
			for (int i = 0; i < stationCount; i++) {
				if (newSt[0] == stations->at(i).getId()) {
					x = i;
					break;
				}
			}
			if (x == -1) {
				cout << "Ошибка" << endl;
				return;
			}
			Station* tempSt = nullptr;
			for (int i = 1; i < newStCount; i++) {
				for (int j = 0; j < stationCount; j++) {
					if (newSt[i - 1] == stations->at(j).getId()) {
						for (int k = 0; k < stations->at(j).getNumAdj(); k++) {
							cout << stations->at(j).getAdjacentStations()[k] << " ";
						}
						cout << endl;
						tempSt = &(stations->at(j));
						break;
					}
				}
				if (!(cin >> newSt[i]).good()) {
					cout << "Ошибка синтаксиса" << endl;
					return;
				}
				bool inside = false;
				for (int j = 0; j < tempSt->getNumAdj(); j++) {
					if (newSt[i] == tempSt->getAdjacentStations()[j]) {
						inside = true;
						break;
					}
				}
				if (!inside) {
					cout << "Нет связи" << endl;
					return;
				}
			}
			temp.changeStations(newSt, newStCount);
			temp.setId(i);
			routes->at(i) = temp;
		}
		write(routes, *n);
	}

	void print(vector<Route>* routes, int n) {
		cout << n << endl;
		for (int i = 0; i < n; i++) {
			cout << routes->at(i) << endl;
		}
	}

	void read(vector<Route>* routes, int* n) {
		ifstream in;
		in.open("routes.txt");
		if (!in.is_open()) {
			cout << "Не удается открыть файл" << endl;
			return;
		}
		in.read(reinterpret_cast<char*>(n), sizeof(int));

		routes->resize(*n);

		in.read(reinterpret_cast<char*>(&(routes->at(0))), sizeof(Route) * *n);
		print(routes, *n);
	}

	void add(vector<Route>* routes, int* n, vector<Station>* stations, int stationCount) {
		Route temp;
		int newStCount;
		cout << "Введите количество станций" << endl;
		if (!(cin >> newStCount).good()) {
			cout << "Ошибка синтаксиса" << endl;
			return;
		}
		if (newStCount > 20) {
			cout << "Ограничение = 20" << endl;
			return;
		}
		int* newSt = new int[newStCount];
		cout << "Введите первый id" << endl;
		if (!(cin >> newSt[0]).good()) {
			cout << "Ошибка синтаксиса" << endl;
			return;
		}
		int x = -1;
		for (int i = 0; i < stationCount; i++) {
			if (newSt[0] == stations->at(i).getId()) {
				x = i;
				break;
			}
		}
		if (x == -1) {
			cout << "Ошибка" << endl;
			return;
		}
		Station* tempSt = nullptr;
		for (int i = 1; i < newStCount; i++) {
			for (int j = 0; j < stationCount; j++) {
				if (newSt[i - 1] == stations->at(j).getId()) {
					for (int k = 0; k < stations->at(j).getNumAdj(); k++) {
						cout << stations->at(j).getAdjacentStations()[k] << " ";
					}
					cout << endl;
					tempSt = &(stations->at(j));
					break;
				}
			}
			if (!(cin >> newSt[i]).good()) {
				cout << "Ошибка синтаксиса" << endl;
				return;
			}
			bool inside = false;
			for (int j = 0; j < tempSt->getNumAdj(); j++) {
				if (newSt[i] == tempSt->getAdjacentStations()[j]) {
					inside = true;
					break;
				}
			}
			if (!inside) {
				cout << "Нет связи" << endl;
				return;
			}
		}
		temp.changeStations(newSt, newStCount);

		temp.setId(routes->at((*n) - 1).getId() + 1);

		routes->push_back(temp);
		(*n)++;
		write(routes, *n);
	}

	void remove(vector<Route>* routes, int* n) {
		print(routes, *n);
		cout << "Введите id для удаления" << endl;
		int id;
		if (!(cin >> id).good()) {
			cout << "Ошибка синтаксиса" << endl;
			return;
		}

		int x = -1;

		for (int i = 0; i < *n; i++) {
			if (routes->at(i).getId() == id) {
				if (routes->at(i).getTrainsCount() > 0) {
					cout << "Нельзя удалять маршруты, к которым прикреплены поезда" << endl;
					return;
				}
				x = i;
				break;
			}
		}
		if (x == -1) {
			cout << "Поиск не удался" << endl;
			return;
		}
		routes->erase(routes->begin() + x);
		(*n)--;
		write(routes, *n);
	}

	void edit(vector<Route>* routes, int n, vector<Station>* stations, int stationCount) {
		print(routes, n);
		cout << "Введите id для редактирования" << endl;
		int id;
		if (!(cin >> id).good()) {
			cout << "Ошибка синтаксиса" << endl;
			return;
		}
		Route* temp = nullptr;
		for (int i = 0; i < n; i++) {
			if (routes->at(i).getId() == id) {
				temp = &(routes->at(i));
				break;
			}
		}
		if (temp == nullptr) {
			cout << "Ошибка ввода" << endl;
			return;
		}
		cout << *temp << endl;

		int key1;
		cout << "1-добавить станцию, 2-изменить станции" << endl;
		if (!(cin >> key1).good()) {
			cout << "Ошибка синтаксиса" << endl;
			return;
		}
		switch (key1) {
		case 1: {
			Station* tempSt = nullptr;
			for (int i = 0; i < stationCount; i++) {
				if (stations->at(i).getId() == temp->getStations()[temp->getStatinsCount() - 1]) {
					tempSt = &(stations->at(i));
					break;
				}
			}
			if (tempSt == nullptr) {
				cout << "Ошибка" << endl;
				return;
			}
			for (int i = 0; tempSt->getNumAdj(); i++) {
				cout << tempSt->getAdjacentStations()[i] << " ";
			}
			cout << endl;
			int x;
			if (!(cin >> x).good()) {
				cout << "Ошибка синтаксиса" << endl;
				return;
			}
			bool inside = false;
			for (int i = 0; i < tempSt->getNumAdj(); i++) {
				if (x == tempSt->getAdjacentStations()[i]) {
					inside = true;
					break;
				}
			}
			if (!inside) {
				cout << "Ошибка" << endl;
				return;
			}
			temp->addStation(x);
			write(routes, n);
		}
			  break;
		case 2: {
			int newStCount;
			cout << "Введите количество станций" << endl;
			if (!(cin >> newStCount).good()) {
				cout << "Ошибка синтаксиса" << endl;
				return;
			}
			if (newStCount > 20) {
				cout << "Станций не может быть больше 20" << endl;
				return;
			}
			int* newSt = new int[newStCount];
			cout << "Введите id первой станции" << endl;
			if (!(cin >> newSt[0]).good()) {
				cout << "Ошибка синтаксиса" << endl;
				return;
			}
			int x = -1;
			for (int i = 0; i < stationCount; i++) {
				if (newSt[0] == stations->at(i).getId()) {
					x = i;
					break;
				}
			}
			if (x == -1) {
				cout << "Такой станции не существует" << endl;
				return;
			}
			Station* tempSt = nullptr;
			for (int i = 1; i < newStCount; i++) {
				for (int j = 0; j < stationCount; j++) {
					if (newSt[i - 1] == stations->at(j).getId()) {
						for (int k = 0; k < stations->at(j).getNumAdj(); k++) {
							cout << stations->at(j).getAdjacentStations()[k] << " ";
						}
						cout << endl;
						tempSt = &(stations->at(j));
						break;
					}
					if (!(cin >> newSt[i]).good()) {
						cout << "Ошибка синтаксиса" << endl;
						return;
					}
					bool inside = false;
					for (int j = 0; j < tempSt->getNumAdj(); j++) {
						if (newSt[i] == tempSt->getAdjacentStations()[j]) {
							inside = true;
							break;
						}
					}
					if (!inside) {
						cout << "Нет связи" << endl;
						return;
					}
				}
			}
			temp->changeStations(newSt, newStCount);
			write(routes, n);
		}
			  break;
		}
	}
}
