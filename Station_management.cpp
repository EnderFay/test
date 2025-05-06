#include "RusRails.h"
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

namespace rjd {
	void write(vector<Station>* station, int n) {
		ofstream out;
		out.open("stations.txt");
		if (!out.is_open()) {
			cout << "Не удается открыть файл" << endl;
			return;
		}
		out.write(reinterpret_cast<char*>(&n), sizeof(int));
		out.write(reinterpret_cast<char*>(&(station->at(0))), sizeof(Station) * n);
		out.close();
	}

	void create(vector<Station>* station, int* n) {
		cout << "n=";
		if (!(cin >> (*n)).good()) {
			cout << "Ошибка синтаксиса" << endl;
			return;
		}
		station->resize((*n));
		for (int i = 0; i < *n; i++) {
			Station temp;
			if (!(cin >> temp).good()) {
				cout << "Ошибка ввода" << endl;
				return;
			}
			temp.setId(i);
			station->at(i) = temp;
		}
		write(station, *n);
	}

	void print(vector<Station>* station, int n) {
		cout << n << endl;
		for (int i = 0; i < n; i++) {
			cout << station->at(i) << endl;
		}
	}

	void read(vector<Station>* station, int* n) {
		ifstream in;
		in.open("stations.txt");
		if (!in.is_open()) {
			cout << "Ошибка при чтении" << endl;
			return;
		}
		in.read(reinterpret_cast<char*>(n), sizeof(int));
		station->resize(*n);
		in.read(reinterpret_cast<char*>(&(station->at(0))), sizeof(Station) * *n);
		print(station, *n);
	}

	void add(vector<Station>* station, int* n) {
		Station temp;
		if (!(cin >> temp).good()) {
			cout << "Что-то пошло не так" << endl;
			return;
		}
		temp.setId(station->at((*n) - 1).getId() + 1);
		station->push_back(temp);
		(*n)++;
		write(station, *n);
	}

	void remove(vector<Station>* station, int* n, vector<Route>* routes, int routesCount) {
		print(station, *n);
		cout << "Введите id для удаления" << endl;
		int id = 0;
		if (!(cin >> id).good()) {
			cout << "Ошибка синтаксиса" << endl;
			return;
		}
		int x = -1;
		for (int i = 0; i < *n; i++) {
			if (station->at(i).getId() == id) {
				for (int j = 0; j < routesCount; j++) {
					for (int k = 0; k < routes->at(j).getStatinsCount(); k++) {
						if (routes->at(j).getStations()[k] == id) {
							cout << "Невозможно удалить станцию" << endl;
							return;
						}
					}
				}
				x = i;
				break;
			}
		}
		if (x == -1) {
			cout << "Поиск не дал результатов" << endl;
			return;
		}
		station->erase(station->begin() + x);
		(*n)--;
		write(station, *n);
	}

	void edit(vector<Station>* station, int n) {
		cout << "Введите id для редактирования" << endl;
		int id;
		if (!(cin >> id).good()) {
			cout << "Ошибка синтаксиса" << endl;
			return;
		}
		Station* temp = nullptr;
		for (int i = 0; i < n; i++) {
			if (station->at(i).getId() == id) {
				temp = &(station->at(i));
				break;
			}
		}
		if (temp == nullptr) {
			cout << "Ошибка ввода" << endl;
			return;
		}

		int stop = 0;
		while (stop != 1) {

			int key1 = 0;
			cout << "1-добавить смежную станцию, 2-удалить смежную станцию, 3-редактировать смежную станцию, 4-изменить название, 5-показать станцию, 6-сохранить и выйти" << endl;
			if (!(cin >> key1).good()) {
				cout << "Ошибка синтаксиса" << endl;
			}
			switch (key1) {
			case 1: {
				cout << "Введите id: ";
				int num = 0;
				if (!(cin >> num).good()) {
					cout << "Ошибка синтаксиса" << endl;
					return;
				}
				cout << "Введите расстояние: ";
				int distance = 0;
				if (!(cin >> distance).good()) {
					cout << "Ошибка синтаксиса" << endl;
					return;
				}
				temp->addAdjacentStation(num, distance);
				break;
			}

			case 2: {
				cout << "Введите id: ";
				int num = 0;
				if (!(cin >> num).good()) {
					cout << "Ошибка синтаксиса" << endl;
					return;
				}

				int index = -1;
				const int* adj = temp->getAdjacentStations();
				for (int i = 0; i < 5; i++) {
					if (i == num) {
						index = adj[i];
						break;
					}
				}

				if (index == -1) {
					cout << "Не удалось найти этот id " << num << endl;
					break;
				}

				temp->removeAdjacentStation(index);
				break;
			}
			case 3: {
				cout << "Введите id: ";
				int index = 0;
				if (!(cin >> index).good()) {
					cout << "Ошибка синтаксиса" << endl;
					return;
				}

				cout << "Введите новый id: ";
				int num_new = 0;
				if (!(cin >> num_new).good()) {
					cout << "Ошибка синтаксиса" << endl;
					return;
				}
				cout << "Введите новое расстояние: ";
				int distance_new = 0;
				if (!(cin >> distance_new).good()) {
					cout << "Ошибка синтаксиса" << endl;
					return;
				}
				temp->editAdjacentStation(index, num_new, distance_new);
				break;
			}
			case 4: {
				cout << "Текущее имя: " << temp->getName() << endl;
				cout << "Новое имя: ";
				char name[100];
				if (!(cin >> name).good()) {
					cout << "Ошибка синтаксиса" << endl;
					return;
				}

				temp->changeName(name);
				break;
			}
			case 5: {
				cout << *temp << endl;
				break;
			}

			case 6: {
				stop = 1;
				break;
			}

			}
		} // end while
		write(station, n);
	}

}
