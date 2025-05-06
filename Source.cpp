#include<iostream>
#include<vector>
#include<ctime>
#include<fstream>
#include <windows.h>
#include"RusRails.h"
using namespace std;

int main() {
	setlocale(LC_ALL, "ru_RU.UTF-8");

	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);
	time_t globalTime;
	int password;
	vector<Route> routes;
	vector<Train> trains;
	vector<Station> stations;
	vector<Ticket> tickets;
	int routesCount = 0;
	int trainsCount = 0;
	int stationCount = 0;
	int ticketsCount = 0;
	ifstream in;
	in.open("settings.txt");
	if (!in.is_open()) {
		cout << "Файл настроек не был найден, производится создание настроек" << endl;
		ofstream out;
		out.open("settings.txt");
		if (!out.is_open()) {
			cout << "Что-то пошло не так" << endl;
			return 1;
		}
		password = 1234;
		out.write(reinterpret_cast<char*>(&password), sizeof(int));
		globalTime = time(NULL);
		cout << globalTime << endl;
		out.write(reinterpret_cast<char*>(&globalTime), sizeof(time_t));
		out.close();
	}
	else {
		in.read(reinterpret_cast<char*>(&password), sizeof(int));
		in.read(reinterpret_cast<char*>(&globalTime), sizeof(time_t));
		in.close();
	}
	tm* normalTime = localtime(&globalTime);
	cout << normalTime->tm_year + 1900 << " " << normalTime->tm_mon + 1 << " " << normalTime->tm_mday << " " << normalTime->tm_hour << endl;
	rjd::read(&tickets, &ticketsCount);

	bool work0 = true;
	while (work0) {
		cout << "1-Вход для администрации 2-Вход для пользователей" << endl;
		int key;
		if (!(cin >> key).good()) {
			cout << "Ошибка синтаксиса" << endl;
			return 1;
		}
		switch (key)
		{
		case 1: {
			cout << "Введите пароль" << endl;
			int pasCheck;
			if (!(cin >> pasCheck).good()) {
				cout << "Ошибка синтаксиса" << endl;
				break;
			}
			if (pasCheck != password) {
				cout << "Пароль введен неверно" << endl;
				break;
			}
			bool work1 = true;
			while (work1) {
				int key1;
				cout << "1-Настройки 2-Работа с БД" << endl;
				if (!(cin >> key1).good()) {
					cout << "Ошибка синтаксиса" << endl;
					break;
				}
				switch (key1)
				{
				case 1:
				{
					cout << "1-изменить пароль, 2-работа с временем" << endl;
					int key2;
					if (!(cin >> key2).good()) {
						cout << "Ошибка синтаксиса" << endl;
						break;
					}
					switch (key2)
					{
					case 1: {
						int newPassword;
						if (!(cin >> newPassword).good()) {
							cout << "Ошибка синтаксиса" << endl;
							break;
						}
						password = newPassword;
						ofstream out;
						out.open("settings.txt");
						out.write(reinterpret_cast<char*>(&password), sizeof(int));
						out.write(reinterpret_cast<char*>(&globalTime), sizeof(time_t));
						out.close();
					}
						  break;
					case 2: {
						cout << "Введите количество часов для добавления" << endl;
						int hours;
						if (!(cin >> hours).good()) {
							cout << "Ошибка синтаксиса" << endl;
							break;
						}
						globalTime += hours * 3600;
						normalTime = localtime(&globalTime);
						cout << normalTime->tm_year + 1900 << " " << normalTime->tm_mon + 1 << " " << normalTime->tm_mday << " " << normalTime->tm_hour << endl;
						ofstream out;
						out.open("settings.txt");
						out.write(reinterpret_cast<char*>(&password), sizeof(int));
						out.write(reinterpret_cast<char*>(&globalTime), sizeof(time_t));
						out.close();
						for (int i = 0; i < trainsCount; i++) {
							trains[i].changeCurStation(globalTime, &routes, routesCount, &stations, &tickets, &ticketsCount);
						}
						rjd::write(&trains, trainsCount);
						rjd::write(&tickets, ticketsCount);
					}
						  break;
					default:
						break;
					}
				}
				break;
				case 2: {
					bool work2 = true;
					while (work2) {
						cout << "1-Поезда 2-Маршруты 3-Станции 4-Вывод билетов" << endl;
						int key2;
						if (!(cin >> key2).good()) {
							cout << "Ошибка синтаксиса" << endl;
							break;
						}
						switch (key2)
						{
							int key3;
						case 1:
						{
							cout << "1-создать 2-читить 3-добавить 4-удалить 5-редактировать" << endl;
							if (!(cin >> key3).good()) {
								cout << "Ошибка синтаксиса" << endl;
								break;
							}
							switch (key3)
							{
							case 1: {
								rjd::create(&trains, &trainsCount, globalTime, &routes, routesCount);
							}
								  break;
							case 2: {
								rjd::read(&trains, &trainsCount);
							}
								  break;
							case 3:
							{
								rjd::add(&trains, &trainsCount, globalTime, &routes, routesCount);
							}
							break;
							case 4: {
								rjd::remove(&trains, &trainsCount, &routes, routesCount);
							}
								  break;
							case 5: {
								rjd::edit(&trains, trainsCount);
							}
								  break;
							default:
								break;
							}
						}
						break;
						case 2: {
							cout << "1-создать 2-читить 3-добавить 4-удалить 5-редактировать" << endl;
							if (!(cin >> key3).good()) {
								cout << "Ошибка синтаксиса" << endl;
								break;
							}
							switch (key3)
							{
							case 1: {
								rjd::create(&routes, &routesCount, &stations, stationCount);
							}
								  break;
							case 2: {
								rjd::read(&routes, &routesCount);
							}
								  break;

							case 3:
							{
								rjd::add(&routes, &routesCount, &stations, stationCount);
							}
							break;
							case 4: {
								rjd::remove(&routes, &routesCount);
							}
								  break;
							case 5: {
								rjd::edit(&routes, routesCount, &stations, stationCount);
							}
								  break;

							default:
								break;
							}
						}
							  break;
						case 3: {
							cout << "1-создать 2-читить 3-добавить 4-удалить 5-редактировать" << endl;
							if (!(cin >> key3).good()) {
								cout << "Ошибка синтаксиса" << endl;
								break;
							}
							switch (key3)
							{
							case 1: {
								rjd::create(&stations, &stationCount);
							}
								  break;
							case 2: {
								rjd::read(&stations, &stationCount);
							}
								  break;

							case 3:
							{
								rjd::add(&stations, &stationCount);
							}
							break;
							case 4: {
								rjd::remove(&stations, &stationCount, &routes, routesCount);
							}
								  break;
							case 5: {
								rjd::edit(&stations, stationCount);
							}
								  break;

							default:
								break;
							}
						}
							  break;
						case 4:
						{
							rjd::print(&tickets, ticketsCount);
						}
						break;
						default:
							work2 = false;
							break;
						}
					}
				}
					  break;
				default:
					work1 = false;
					break;
				}
			}
		}
			  break;
		case 2: {
			if (stationCount == 0)
				rjd::read(&stations, &stationCount);
			if (routesCount == 0)
				rjd::read(&routes, &routesCount);
			if (trainsCount == 0)
				rjd::read(&trains, &trainsCount);
			Matrix matrix(stationCount, stationCount);
			for (int i = 0; i < stationCount; i++) {
				for (int j = 0; j < stationCount; j++) {
					int x = 0;
					for (int k = 0; k < stations[i].getNumAdj(); k++) {
						if (j == stations[i].getAdjacentStations()[k]) {
							x = 1;
							break;
						}
					}
					matrix[i][j] = x;
				}
			}
			Matrix powerMatrix = matrix.sumOfPowers(stationCount);
			cout << "Выберите первую станцию" << endl;
			int stA;
			int a;
			if (!(cin >> stA).good()) {
				cout << "Ошибка синтаксиса" << endl;
				break;
			}
			bool inside = false;
			for (int i = 0; i < stationCount; i++) {
				if (stA == stations[i].getId()) {
					a = i;
					inside = true;
					break;
				}
			}
			if (!inside) {
				cout << "Ошибка" << endl;
				break;
			}
			cout << "Выберите конечную станцию" << endl;
			int stB;
			int b;
			if (!(cin >> stB).good()) {
				cout << "Ошибка синтаксиса" << endl;
				break;
			}
			inside = false;
			for (int i = 0; i < stationCount; i++) {
				if (stB == stations[i].getId()) {
					b = i;
					inside = true;
					break;
				}
			}
			if (!inside) {
				cout << "Ошибка" << endl;
				break;
			}
			if (powerMatrix[a][b] == 0) {
				cout << "Нет связи от " << stA << " до " << stB << endl;
				break;
			}
			vector<Route> correct;
			for (int i = 0; i < routesCount; i++) {
				bool insideA = false;
				bool insideB = false;
				for (int j = 0; j < routes[i].getStatinsCount(); j++) {
					if (routes[i].getStations()[j] == stA) {
						insideA = true;
					}
					else if (routes[i].getStations()[j] == stB) {
						insideB = true;
					}
				}
				if (insideA && insideB) {
					correct.push_back(routes[i]);
				}
			}
			if (correct.empty()) {
				cout << "В данной линии нет соединения между нужными станциями" << endl;
				break;
			}
			vector<Train*> fin;
			for (int i = 0; i < correct.size(); i++) {
				bool inside = false;
				for (int j = 0; j < correct[i].getTrainsCount(); j++) {
					for (int k = 0; k < trainsCount; k++) {
						if (trains[k].getId() == correct[i].getTrains()[j]) {
							inside = true;
							Train* temp = trains[k].getPasAndTime(&tickets, ticketsCount, &stations, stationCount, &routes, routesCount, stA, stB, globalTime);
							fin.push_back(temp);
							break;
						}
					}
				}
			}
			cout << "Выберите поезд" << endl;
			int trainId;
			if (!(cin >> trainId).good()) {
				cout << "Ошибка синтаксиса" << endl;
				break;
			}
			inside = false;
			for (int i = 0; i < fin.size(); i++) {
				if (fin[i]->getId() == trainId) {
					inside = true;
					break;
				}
			}
			if (!inside) {
				cout << "Ошибка" << endl;
				break;
			}
			Ticket newTicket;
			newTicket.setTrain(trainId);
			newTicket.setFinish(stB);
			newTicket.setStart(stA);
			cin >> newTicket;

			rjd::add(&tickets, &ticketsCount, newTicket);
		}
			  break;
		default:
			break;
		}
	}
}
