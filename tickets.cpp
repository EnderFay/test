#include<iostream>
#include<ctime>
#include<fstream>
#include<string>
#include"RusRails.h"

using namespace std;

istream& operator >>(istream& in, Ticket& r) {
	cout << "ФИО=";
	string str;
	in.get();
	getline(in, str);
	if (str.length() <= 40) {
		for (int i = 0; i < str.length(); i++) {
			r.name[i] = str[i];
		}
		for (int i = str.length(); i < 40; i++) {
			r.name[i] = NULL;
		}
	}
	else {
		for (int i = 0; i < 40; i++) {
			r.name[i] = str[i];
		}
	}
	
	return in;
}

ostream& operator <<(ostream& out, const Ticket& r) {
	out << r.id << " " << r.name << " " << r.startSt << " " << r.trainId << " " << r.finish;
	return out;
}

void Ticket::setId(int x) {
	id = x;
}

int Ticket::getId() {
	return id;
}

int Ticket::getFinish() {
	return finish;
}

int Ticket::getStart() {
	return startSt;
}

int Ticket::getTrain() {
	return trainId;
}

void Ticket::setTrain(int x) {
	trainId = x;
}

void Ticket::setStart(int x) {
	startSt = x;
}

void Ticket::setFinish(int x) {
	finish = x;
}
