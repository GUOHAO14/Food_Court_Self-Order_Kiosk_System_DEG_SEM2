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
	bool hasSuitableStall = false;
	bool assigned = false;

	struct food* currentFood = order->getFoodList()->getHead();

	for (int i = 0; i < order->getFoodList()->getCount(); i++) {
		// process each food item in the order
		if (currentFood == nullptr) {
			cout << "No food items in the order." << endl;
			break;
		}
		
		// if all stalls are either closed or cannot prepare the food, count as impossible to assign
		// if all stalls impossible handle this food, this food will be dropped from order

		for (int k = 0; k < stallCircularQueue.getMaxStalls(); k++) {
			struct stall* currentStall = stallCircularQueue.getQueue()[k];

			if (currentStall != nullptr) {

				// stall not suitable for this food
				if (!foodStallMapList.checkFoodStallMapping(currentFood->id, currentStall->id)) {
					continue;
				}

				// suitable stall is open and free
				else if (currentStall->isOpen && !currentStall->foodQueue.isFull() && foodStallMapList.checkFoodStallMapping(currentFood->id, currentStall->id)) {

					hasSuitableStall = true;
					assigned = true;

					cout << currentStall->name << " stall is open and not busy. Assigning order to this stall." << endl;

					// assign order to the stall
					currentStall->foodQueue.enqueueFood(currentFood);

					currentStall->foodQueue.displayAllOrders(); // Display the orders in the stall's queue

					cout << "Count: " << currentStall->foodQueue.getCount() << endl;

					// then dequeue the stall in order assignment
					stallCircularQueue.dequeueStall();
					// re-enqueue the stall to the end of the circular queue 
					// to achieve rotate mechanism for load balancing
					stallCircularQueue.enqueueStall(currentStall);

					break; // Exit after assigning the order
				}

				// suitable stall that is open but not free, or is closed
				else if (foodStallMapList.checkFoodStallMapping(currentFood->id, currentStall->id) && ((currentStall->isOpen && currentStall->foodQueue.isFull()) || !currentStall->isOpen)) {

					hasSuitableStall = true;
					assigned = false;

					cout << currentStall->name << " stall is busy or closed. Moving to unassigned waiting queue." << endl;

					unassignedFoodQueue.insertRear(currentFood);

				}

				else {
					cout << "An error occured, stall is skipped. Moving to the next stall." << endl;
				}
			}
			else {
				cout << "No stall located." << endl;
			}
		}

		if (hasSuitableStall && !assigned) {
			cout << "Final Verdict:" << endl;
			cout << "Suitable stall found for food item: " << currentFood->name << endl;
			cout << "But the stall is busy or closed." << endl;
			cout << "Moving to unassigned waiting queue." << endl;
			unassignedFoodQueue.insertRear(currentFood);

			cout << "Current unassigned waiting queue:" << endl;
			unassignedFoodQueue.displayAllFood();
		}

		if (!hasSuitableStall && !assigned) {
			cout << "All stalls are either closed or busy." << endl;
			cout << "Cannot assign your food at this time." << endl;
			cout << "Order will remain in waiting queue as pending." << endl;
		}

		stallCircularQueue.displayQueue();
		currentFood = currentFood->next;
	}
};