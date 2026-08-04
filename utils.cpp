#include "utils.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// define function implementations
// retrieve food data from csv file, store in linked list
void loadFoodFromCSV(Food_Linked_List* food, string fileName) {
	ifstream file(fileName);

	if (!file.is_open())
	{
		cout << "Failed to open \"" << fileName << "\"" << endl;
		return;
	}

	string line;
	getline(file, line); //skip first row (header) of csv

	while (getline(file, line))
	{
		stringstream ss(line);

		string food_id, name, food_price;

		getline(ss, food_id, ',');
		getline(ss, name, ',');
		getline(ss, food_price, ',');

		// convert id and price to int and double
		int id = stoi(food_id);
		double price = stod(food_price);

		food->insertRear(id, name, price);
	}

	file.close();
};

// retrieve stall data from csv file, store in linked list
void loadStallFromCSV(Stall_Linked_List* stall, string fileName) {
	ifstream file(fileName);

	if (!file.is_open())
	{
		cout << "Failed to open \"" << fileName << "\"" << endl;
		return;
	}

	string line;
	getline(file, line); //skip first row (header) of csv

	while (getline(file, line))
	{
		stringstream ss(line);

		string stall_id, name, is_open;

		getline(ss, stall_id, ',');
		getline(ss, name, ',');
		getline(ss, is_open, ',');

		// convert id to int
		int id = stoi(stall_id);

		// convert is_open to boolean
		bool isOpen = (is_open == "true");

		stall->insertRear(id, name, isOpen);
	}

	file.close();
};

// retrieve food stall mapping data from csv file, store in linked list
void loadFoodStallMapFromCSV(Food_Stall_Map_Linked_List* map, string fileName) {
	ifstream file(fileName);

	if (!file.is_open())
	{
		cout << "Failed to open \"" << fileName << "\"" << endl;
		return;
	}

	string line;
	getline(file, line); //skip first row (header) of csv

	while (getline(file, line))
	{
		stringstream ss(line);

		string food_id, stall_id;

		getline(ss, food_id, ',');
		getline(ss, stall_id, ',');

		// convert id from string to int
		int food_id_int = stoi(food_id);
		int stall_id_int = stoi(stall_id);

		map->insertRear(food_id_int, stall_id_int);
	}

	file.close();
};