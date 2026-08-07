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

	// int var used for circular queue position (array)
	int position = 0;

	while (getline(file, line))
	{
		stringstream ss(line);

		string stall_id, name, is_open, circular_queue_position;

		getline(ss, stall_id, ',');
		getline(ss, name, ',');
		getline(ss, is_open, ',');
		getline(ss, circular_queue_position, ',');

		// convert id to int
		int id = stoi(stall_id);

		// convert is_open to boolean
		bool isOpen = (is_open == "true");

		stall->insertRear(id, name, isOpen);

		// build back stall circular queue for food assignment
		// stallCircularQueue already decalred in globals.h, defined before this function execute in main
		stallCircularQueue.insertStallAtPosition(stall->searchStallById(id), position);

		if (circular_queue_position == "rear") {
			stallCircularQueue.setRear(position);
		} 
		else if (circular_queue_position == "front") {
			stallCircularQueue.setFront(position);
		}

		position++;
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

void loadOrderFromCSV(Queue* pending, Queue* processing, Queue* completed, Food_Linked_List* foodList, string orderFile, string mapfoodFile) {
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
		string order_id, stu_id, order_time, order_status;
		getline(ss, order_id, ',');
		getline(ss, stu_id, ',');
		getline(ss, order_time, ',');
		getline(ss, order_status, ',');
		int id = stoi(order_id);
		int student_id = stoi(stu_id);

		if (order_status == "Pending")
			pending->addQueue(new Order(id, student_id, order_time, order_status));
		else if (order_status == "Processing")
			processing->addQueue(new Order(id, student_id, order_time, order_status));
		else if (order_status == "Completed")
			completed->addQueue(new Order(id, student_id, order_time, order_status));
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

		string order_id, food_id, status, stall_id;
		getline(ss, order_id, ',');
		getline(ss, food_id, ',');
		getline(ss, status, ',');
		getline(ss, stall_id, ',');

		int o_id = stoi(order_id);
		int f_id = stoi(food_id);
		int stallId = stoi(stall_id);

		food* originalFood = foodList->searchFoodById(f_id);
		food* selectedFood = new food(f_id, originalFood->name, originalFood->price, status);
		Order* selectedOrder = pending->searchOrderById(o_id);
		if (!selectedOrder) {
			selectedOrder = processing->searchOrderById(o_id);

			if (!selectedOrder) {
				selectedOrder = completed->searchOrderById(o_id);
			}
		}

		if (selectedOrder) {

			selectedOrder->addFood(selectedFood);

			if (stallId != 0 && selectedFood->status == "Processing") {
				struct stall* stall = stallList.searchStallById(stallId);

				struct foodForAssignment* newSoloFoodOrder = new foodForAssignment(selectedOrder, selectedFood);

				stall->internalFoodList.insertRear(newSoloFoodOrder);
			}
		}
	}
	newfile.close();
};

void saveOrderToCSV(Queue* pending, Queue* processing, Queue* completed, Stall_Linked_List* stallList, string orderFile, string mapfoodFile) {
	ofstream orderOut(orderFile);
	ofstream mapOut(mapfoodFile);
	if (!orderOut.is_open())
	{
		cout << "Failed to open \"" << orderFile << "\"" << endl;
		return;
	}
	if (!mapOut.is_open())
	{
		cout << "Failed to open \"" << mapfoodFile << "\"" << endl;
		orderOut.close();
		return;
	}

	orderOut << "order_id,student_id,order_time,order_status\n";
	mapOut << "order_id,food_id,status,stall\n";

	//pending->saveOrders(queue, orderOut, mapOut);
	//processing->saveOrders(orderOut, mapOut, stallList);
	//completed->saveOrders(orderOut, mapOut);

	saveOrders(orderOut, mapOut, pending);
	saveOrders(orderOut, mapOut, processing, stallList);
	saveOrders(orderOut, mapOut, completed);

	orderOut.close();
	mapOut.close();
}
// , Stall_Linked_List* stallList
// for individual queue
void saveOrders(ofstream& orderOut, ofstream& mapOut, Queue* queue, Stall_Linked_List* stallList)
{
	Node* currentNode = queue->getHead();

	while (currentNode != nullptr)
	{
		Order* order = currentNode->data;

		orderOut << order->getOrderID() << ","
			<< order->getStudentID() << ","
			<< order->getOrderTime() << ","
			<< order->getOrderStatus() << endl;

		saveOrderMapFood(mapOut, order->getOrderID(), order, stallList);

		currentNode = currentNode->next;
	}
}

void saveOrderMapFood(ofstream& out, int orderId, Order* order, Stall_Linked_List* stallList) {
	struct food* current = order->getFoodList()->getHead();

	int stallId = 0;

	while (current != nullptr) {
		// go through each food in an order
		int foodId = current->id;

		// default means no stall is currently preparing this food
		int stallId = 0;

		if (stallList != nullptr) {

			// go through every stall
			stall* currentStall = stallList->getHead();

			while (currentStall != nullptr) {

				// ask this stall whether it is currently handling
				// this (order + food) assignment
				if (currentStall->internalFoodList.containsFoodAssignment(orderId, foodId)) {

					// stall found
					stallId = currentStall->id;
					break;
				}

				currentStall = currentStall->next;
			}
		}

		// save into csv
		out << orderId << "," << current->id << "," << current->status << "," << stallId << endl;

		current = current->next;
	}
}