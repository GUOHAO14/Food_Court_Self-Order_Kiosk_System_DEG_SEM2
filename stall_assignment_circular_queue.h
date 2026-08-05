#pragma once
#include <iostream>
#include <iomanip>
#include "stall.h"

using namespace std;

// circular queue for checking all stalls
// check whether they can prepare the food, they are free, and they are open
// also ensure order workload is balanced between stalls
class Stall_Assignment_Circular_Queue {
private:
	struct stall* queue[5];
	int maxStalls = 5, front = -1, rear = -1, count = 0;

public:
	Stall_Assignment_Circular_Queue() {

	}

	stall** getQueue() {
		return queue;
	}

	void setFront(int f) {
		front = f;
	}

	void setRear(int r) {
		rear = r;
	}

	int getMaxStalls() {
		return maxStalls;
	}

	bool isFull() {
		if (count == maxStalls)
			return true;
		return false;
	}

	int getFront() {
		return front;
	}

	int getRear() {
		return rear;
	}

	bool isEmpty()
	{
		if (front == -1 && rear == -1)
			return true;
		return false;
	}

	void enqueueStall(struct stall* stall)
	{
		if (front == -1) {
			front = 0; // front initialisation
		}
		rear = (rear + 1) % maxStalls; // flow back to front if rear reaches maxOrders
		queue[rear] = stall;
		count++;
	}

	void dequeueStall()
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
		count--;
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
	void displayQueue() {
		cout << "^^ = front = " << front << endl;
		cout << "^ = rear = " << rear << endl;

		for (int i = 0; i < maxStalls; i++) {
			cout << setw(12) << queue[i]->name << " ";
		}
		cout << endl;
		for (int i = 0; i < maxStalls; i++) {
			if (i == front) {
				cout << setw(13) << "^^";
			}
			else if (i == rear) {
				cout << setw(13) << "^";
			}
			else {
				cout << setw(13) << " ";
			}
		}
		cout << endl;
	}

	// for loading circular queue from CSV
	void insertStallAtPosition(stall* stall, int position)
	{
		queue[position] = stall;
	}
};