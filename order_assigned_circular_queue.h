#pragma once
#include <iostream>
using namespace std;

class Order_Assigned_Circular_Queue {
	int queue[8];
	int maxOrders = 8, front = -1, rear = -1;

public:
	bool isEmpty()
	{
		if (front == -1 && rear == -1)
			return true;
		return false;
	}

	void enqueue(int data)
	{
		if (front == -1) {
			front = 0; // front initialisation
		}
		rear = (rear + 1) % maxOrders; // flow back to front if rear reaches maxOrders
		queue[rear] = data;
	}

	void dequeue()
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

	void display() {
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
};