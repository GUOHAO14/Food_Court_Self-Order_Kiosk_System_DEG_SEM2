#pragma once
#include "food.h"
#include "stall.h"
#include "food_stall_map.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void loadFoodFromCSV(Food_Linked_List* food, string fileName);
void loadStallFromCSV(Stall_Linked_List* stall, string fileName);
void loadFoodStallMapFromCSV(Food_Stall_Map_Linked_List* map, string fileName);

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

		int id = stoi(food_id);
		double price = stod(food_price);

		food->insertRear(id, name, price);
	}

	file.close();
};

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

		string stall_id, name;

		getline(ss, stall_id, ',');
		getline(ss, name, ',');

		int id = stoi(stall_id);

		stall->insertRear(id, name);
	}

	file.close();
};

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


		int food_id_int = stoi(food_id);
		int stall_id_int = stoi(stall_id);

		map->insertRear(food_id_int, stall_id_int);
	}

	file.close();
};