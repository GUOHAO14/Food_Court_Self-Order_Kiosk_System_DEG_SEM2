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

void loadOrderFromCSV(Queue* queue, Food_Linked_List* foodList, string orderFile, string mapfoodFile) {
	//Load Order Details
	ifstream file(orderFile);
	if (!file.is_open())
	{
		cout << "Failed to open \"" << orderFile << "\"" << endl;
		return;
	}

	string line;
	getline(file, line);
	while (getline(file, line))
	{
		stringstream ss(line);
		string order_id, stu_id, order_time;
		getline(ss, order_id, ',');
		getline(ss, stu_id, ',');
		getline(ss, order_time, ',');
		int id = stoi(order_id);
		int student_id = stoi(stu_id);

		queue->addQueue(Order(id, student_id, order_time));
	}
	file.close();

	//Load Foods in the Order
	ifstream newfile(mapfoodFile);
	if (!newfile.is_open())
	{
		cout << "Failed to open \"" << mapfoodFile << "\"" << endl;
		return;
	}

	string newline;
	getline(newfile, newline);
	while (getline(newfile, newline))
	{
		stringstream ss(newline);

		string order_id, food_id;
		getline(ss, order_id, ',');
		getline(ss, food_id, ',');
		int o_id = stoi(order_id);
		int f_id = stoi(food_id);

		food* selectedFood = foodList->searchFoodById(f_id);
		Order* selectedOrder = queue->searchOrderById(o_id);
		selectedOrder->addFood(*selectedFood);
	}
	newfile.close();
};