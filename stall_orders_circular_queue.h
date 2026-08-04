#pragma once
#include <iostream>
using namespace std;

class Stall_Orders_Circular_Queue {
	int queue[8];
	int maxOrders = 8, front = -1, rear = -1;

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
		rear = (rear + 1) % maxOrders; // flow back to front if rear reaches maxOrders
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
			front = (front + 1) % maxOrders;
		}
	}

	void displayAllOrders() {
		if (isEmpty()) {
			cout << "Queue is empty" << endl;
			return;
		}

		cout << " Items: ";

		for (int i = 0; i < maxOrders; i++)
		{
			if (queue[i] != NULL)
				cout << queue[i] << " , ";
			else
				cout << "NULL , ";
		}

		cout << endl;
	}

	// header for stall display
	void displayStallsHeader() {
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
	void displayStallsFormat(int count, struct stall* s) {
		cout << "| ";
		cout << left << setw(3) << count;
		cout << " | ";
		cout << left << setw(8) << s->id;
		cout << " | ";
		cout << left << setw(20) << s->name;
		cout << " | ";
		cout << left << setw(10) << (s->isOpen ? "Open" : "Closed");
		cout << " |" << endl;
	}

	// display all stalls in a formatted table
	void displayAllStalls() {
		//struct stall* trav = head;
		//int count = 0;
		//displayStallsHeader();
		//while (trav != nullptr) {
		//	count++;
		//	displayStallsFormat(count, trav);
		//	trav = trav->next;
		//}
		//cout << string(54, '=') << endl << endl;
	}
};