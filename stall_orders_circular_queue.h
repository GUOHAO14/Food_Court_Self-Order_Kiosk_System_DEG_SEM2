#pragma once
#include <iostream>
#include "queue.h"
using namespace std;

// different from order assignment
// this class is circular queue for the stall itself
// max orders intake is 6. So 6 orders in queue indicate that the stall is busy
// this class also allows stall owner/staff to mark an order as complete
// hence dequeueing and freeing up the stall for new orders
class Stall_Orders_Circular_Queue {
private:
	Order* queue[6];
	int maxOrders = 6, front = -1, rear = -1;

public:
	bool isEmpty()
	{
		if (front == -1 && rear == -1)
			return true;
		return false;
	}

	void enqueueOrder(Order* fullOrder)
	{
		if (front == -1) {
			front = 0; // front initialisation
		}
		rear = (rear + 1) % maxOrders; // flow back to front if rear reaches maxOrders
		queue[rear] = fullOrder;
	}

	void dequeueOrder()
	{
		if (isEmpty()) {
			cout << "Queue is empty" << endl;
			return;
		}

		if (front == rear) {
			front = -1; // reset front and rear to -1 to indicate empty queue
			rear = -1;
		}
		else {
			queue[front] = NULL; // remove element in the front
			front = (front + 1) % maxOrders;
		}
	}

	// header for stall display
	void displayOrderQueueHeader() {
		cout << string(54, '=') << endl;
		cout << "| ";
		cout << left << setw(3) << "No.";
		cout << " | ";
		cout << left << setw(8) << "Order ID";
		cout << " | ";
		cout << left << setw(8) << "Food ID";
		cout << " | ";
		cout << left << setw(20) << "Food Name";
		cout << " | ";
		cout << left << setw(10) << "Status";
		cout << " | ";
		cout << endl;
		cout << string(54, '=') << endl;
	}

	// actual display for stall data
	//void displayStallsFormat(int count, struct food* f) {
	//	cout << "| ";
	//	cout << left << setw(3) << count;
	//	cout << " | ";
	//	cout << left << setw(8) << f->id;
	//	cout << " | ";
	//	cout << left << setw(20) << s->name;
	//	cout << " | ";
	//	cout << left << setw(10) << (s->isOpen ? "Open" : "Closed");
	//	cout << " |" << endl;
	//}

	// display all stalls in a formatted table
	void displayAllStalls() {
		displayOrderQueueHeader();
		for (int i = 0; i < maxOrders; i++) {
			cout << queue[i] << " ";
		}
		cout << string(54, '=') << endl << endl;
	}

	void tempDisplay() {
		for (int i = 0; i < maxOrders; i++) {
			if (queue[i] != NULL) {
				cout << "Order ID: " << queue[i]->getOrderID() << endl;
				cout << "Student ID: " << queue[i]->getStudentID() << endl;
				cout << "Order Time: " << queue[i]->getOrderTime() << endl;
				queue[i]->getFoodList().displayAllFood();
				cout << "------------------------" << endl;
			}
		}
	}
};