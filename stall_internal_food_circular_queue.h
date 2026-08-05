#pragma once
#include <iostream>
#include "queue.h"
using namespace std;

// different from order assignment
// this class is circular queue for the stall itself
// max orders intake is 6. So 6 orders in queue indicate that the stall is busy
// this class also allows stall owner/staff to mark an order as complete
// hence dequeueing and freeing up the stall for new orders
class Stall_Internal_Food_Circular_Queue {
private:
	food* queue[6];
	int maxFoodOrder = 6, front = -1, rear = -1, count = 0;

public:
	bool isFull() {
		if (count == maxFoodOrder)
			return true;
		return false;
	}

	bool isEmpty()
	{
		if (count == 0)
			return true;
		return false;
	}

	int getCount() {
		return count;
	}

	void enqueueFood(food* foodOrder)
	{
		if (front == -1) {
			front = 0; // front initialisation
		}
		rear = (rear + 1) % maxFoodOrder; // flow back to front if rear reaches maxOrders
		queue[rear] = foodOrder;
		count++;
	}

	void dequeueFood()
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
			front = (front + 1) % maxFoodOrder;
		}
		count--;
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
	void displayAllOrders() {
		if (isEmpty()) {
			cout << "No orders to show. Circular queue is empty!" << endl;
			return;
		}
		//for (int i = 0; i < count; i++) {
		//	cout << "Order ID : " << queue[i]->getOrderID() << endl;
		//	cout << "Student  : " << queue[i]->getStudentID() << endl;
		//	cout << "Time     : " << queue[i]->getOrderTime() << endl;

		//	queue[i]->getFoodList().displayAllFood(true);
		//}
		//cout << string(54, '=') << endl << endl;
	}
};