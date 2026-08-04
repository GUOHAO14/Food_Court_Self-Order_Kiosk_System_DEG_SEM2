#pragma once
#include <iostream>
using namespace std;

// circular queue for checking all stalls
// check whether they can prepare the food, they are free, and they are open
// also ensure order workload is balanced between stalls
class Order_Assignment_Circular_Queue {
	int queue[5];
	int maxStalls = 5, front = -1, rear = -1;

public:
	bool isEmpty()
	{
		if (front == -1 && rear == -1)
			return true;
		return false;
	}

	void enqueueOrder(int data)
	{
		if (front == -1) {
			front = 0; // front initialisation
		}
		rear = (rear + 1) % maxStalls; // flow back to front if rear reaches maxOrders
		queue[rear] = data;
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
			front = (front + 1) % maxStalls;
		}
	}

	void displayAllOrders() {
		if (isEmpty()) {
			cout << "Queue is empty" << endl;
			return;
		}

		cout << " Items: ";

		for (int i = 0; i < maxStalls; i++)
		{
			if (queue[i] != NULL)
				cout << queue[i] << " , ";
			else
				cout << "NULL , ";
		}

		cout << endl;
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
};