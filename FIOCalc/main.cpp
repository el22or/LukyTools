/*
	Small concole app to demonstrate the calculation-method of the new PPS-FIO Membership
	Copyright (C) 2012  Lukas Zurschmiede <l.zurschmiede@delightsoftware.com>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>

using namespace std;

void createSections();
void calculate(map<string, float> *copy, float amount);

float sum;
map<string, float> sections;
map<string, float> payments;

int main(int argc, char **argv) {
	// Floating point output format to standard and not scientific
	cout.setf(ios::fixed, ios::floatfield);
	cout.setf(ios::showpoint);
	cout.precision(2);
	
	createSections();
	map<string, float> *copy = new map<string, float>();
	map<string, float>::const_iterator it;
	string line;
	float amount;
	
	// get the recomment total
	for (it = sections.begin(); it != sections.end(); it++) {
		sum += it->second;
	}
	
	while (true) {
		// copy sections to calculate different paid amounts after and show an overview
		cout << endl << "Recommendations:" << endl;
		cout << "-----------------------" << endl;
		for (it = sections.begin(); it != sections.end(); it++) {
			copy->insert( pair<string, float>(it->first, it->second) );
			cout << " " << it->first << ":\t\t" << it->second << endl;
		}
		cout << "-----------------------" << endl;
		cout << " Recommend:\t" << sum << endl;
		cout << "=======================" << endl << endl;
		
		// Get paid amount or exit
		cout << "Amount paid (Empty to quit): ";
		getline(cin, line);
		amount = atof(line.c_str());
		if (!amount) {
			break;
		}
		cout << endl;
		
		// calculate section contributions and show them
		payments.clear();
		calculate(copy, amount);
		
		cout << endl << "Payments:" << endl;
		cout << "-----------------------" << endl;
		for (it = payments.begin(); it != payments.end(); it++) {
			cout << " " << it->first << ":\t\t" << it->second << endl;
		}
		cout << "-----------------------" << endl;
		cout << " Paid:\t\t" << amount << endl;
		cout << "=======================" << endl << endl;
	}
	return 0;
}

/**
* Recursive call to calculate all amounts from all sections
* 
* @param *copy Outstanding sections
* @param amount Outstanding amount
*/
void calculate(map<string, float> *copy, float amount) {
	if (amount <= 0) {
		return;
	}
	
	if (copy->size() > 0) {
		map<string, float>::const_iterator it;
		string least;
		float due = 0, smallest = 0;
		
		// If the amount is bigger than the recommendend sum, give each section the procentula amount of it
		if (amount >= sum) {
			for (it = copy->begin(); it != copy->end(); it++) {
				payments.insert( pair<string, float>(it->first, amount * (it->second / sum)) );
			}
			copy->clear();
			return;
		}
		
		// get the smallest amount and the current due amount
		for (it = copy->begin(); it != copy->end(); it++) {
			smallest = smallest <= 0 || it->second < smallest ? it->second : smallest;
		}
		due = amount / copy->size();
		
		// if the due is smaller than the smallest amount, give the same amount to each unpaid section
		if (due <= smallest) {
			for (it = copy->begin(); it != copy->end(); it++) {
				payments.insert( pair<string, float>(it->first, due) );
			}
			copy->clear();
			return;
		}
		
		// pay the smallest section and rerun this calculation with all other sections expect the one we pay here
		for (it = copy->begin(); it != copy->end(); it++) {
			if (due >= it->second) {
				payments.insert( pair<string, float>(it->first, it->second) );
				amount -= it->second;
				copy->erase(it->first);
			}
		}
		calculate(copy, amount);
	}
}

/**
* User can input a SectionName and an amount after
* finished by enter an empty string as Sectionname
*/
void createSections() {
	string line1, line2;
	float amount;
	while (true) {
		cout << endl << "Enter the name of the Section or just press Enter finish: ";
		getline(cin, line1);
		
		if (line1.empty()) {
			break;
		}
		
		amount = 0.0;
		while (amount <= 0) {
			cout << "Recommend amount: ";
			getline(cin, line2);
			amount = atof(line2.c_str());
			if (amount <= 0) {
				cout << endl << endl << "invalid, write a valid float/double..." << endl;
			}
		}
		sections.insert( pair<string, float>(line1, amount) );
	}
}
