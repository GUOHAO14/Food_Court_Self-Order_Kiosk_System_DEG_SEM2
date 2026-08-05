#include <iostream>
#include "globals.h"
#include "stall_assignment.h"

using namespace std;

Order_Assignment_Circular_Queue stallCircularQueue;

void stallAndOrderAssignment(Order * order) {
	stallCircularQueue = Order_Assignment_Circular_Queue(); 
	// Reset the circular queue for each new order to be assigned

	stallCircularQueue.displayQueue();

	// queue status
	bool assigned = false;

	for (int i = 0; i < stallCircularQueue.getMaxStalls(); i++) {
		struct stall* currentStall = stallCircularQueue.getQueue()[i];

		if (currentStall != nullptr) {

			if (currentStall->isOpen && !currentStall->foodQueue.isFull()) {
				cout << currentStall->name << " stall is open and not busy. Assigning order to this stall." << endl;
				assigned = true;
				// assign order to the stall
				currentStall->foodQueue.enqueueFood(order);

				currentStall->foodQueue.displayAllOrders(); // Display the orders in the stall's queue

				cout << "Count: " << currentStall->foodQueue.getCount() << endl;

				// then dequeue the stall in order assignment
				stallCircularQueue.dequeueStall();
				// re-enqueue the stall to the end of the circular queue 
				// to achieve rotate mechanism for load balancing
				stallCircularQueue.enqueueStall(currentStall);

				break; // Exit after assigning the order
			}
			else {
				cout << currentStall->name << " stall is either closed or busy. Moving to the next stall." << endl;
			}
		}
		else {
			cout << "No stall located." << endl;
		}
	}
	if (!assigned) {
		cout << "All stalls are either closed or busy. Cannot assign order at this time." << endl;
	}

	stallCircularQueue.displayQueue();
};