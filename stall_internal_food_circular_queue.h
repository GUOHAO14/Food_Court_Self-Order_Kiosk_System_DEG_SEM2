#pragma once
#include <iostream>
#include "queue.h"
#include "food.h"
#include "food_stall_map.h"
#include "food_for_assignment.h"
using namespace std;

// different from order assignment
// this class is circular queue for the stall itself
// max orders intake is 6. So 6 orders in queue indicate that the stall is busy
// this class also allows stall owner/staff to mark an order as complete
// hence dequeueing and freeing up the stall for new orders
class Stall_Internal_Food_Circular_Queue {
private:
	foodForAssignment* queue[6];
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

	foodForAssignment** getQueue()
	{
		return queue;
	}

	void enqueueFoodAssignment(foodForAssignment* foodOrder)
	{
		if (front == -1) {
			front = 0; // front initialisation
		}
		rear = (rear + 1) % maxFoodOrder; // flow back to front if rear reaches maxOrders
		queue[rear] = foodOrder;
		count++;
	}

	void dequeueFoodAssignment()
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

	bool markComplete(int queueIndex) {
		if (queueIndex < 0 || queueIndex > count) {
			cout << "Invalid food. Index out of bound." << endl;
			return false;
		}

		queue[queueIndex]->food->status = "Completed";

		cout << "Food marked as completed" << endl;

		return true;
	}

	// display all stalls in a formatted table
	void displayFoodQueue() {
		if (isEmpty()) {
			cout << "No orders to show. Circular queue is empty!" << endl;
			return;
		}
		int length = 62;
		Food_For_Assignment_Linked_List().displayFoodAssignmentHeader(length);
		for (int i = 0; i < count; i++) {
			Food_For_Assignment_Linked_List().displayFoodAssignmentFormat(i+1, queue[i]);
		}
		cout << string(length, '=') << endl << endl;
	}
};