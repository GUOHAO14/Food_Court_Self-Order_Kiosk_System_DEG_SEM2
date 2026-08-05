#include <iostream>
#include "globals.h"
#include "stall_assignment.h"

using namespace std;

Order_Assignment_Circular_Queue stallCircularQueue;
Food_Linked_List* unassignedFoodQueue = new Food_Linked_List();

void stallAndOrderAssignment(Order * order) {
	stallCircularQueue = Order_Assignment_Circular_Queue(); 
	// Reset the circular queue for each new order to be assigned

	stallCircularQueue.displayQueue();

	// queue status
	bool assigned = false;
	struct food* currentFood = order->getFoodList()->getHead();

	for (int i = 0; i < order->getFoodList()->getCount(); i++) {
		// process each food item in the order
		if (currentFood == nullptr) {
			cout << "No food items in the order." << endl;
			break;
		}

		int impossibleToAssignCount = 0; 
		// if all stalls are either closed or cannot prepare the food, count as impossible to assign
		// if all stalls impossible handle this food, this food will be dropped from order

		for (int i = 0; i < stallCircularQueue.getMaxStalls(); i++) {
			struct stall* currentStall = stallCircularQueue.getQueue()[i];

			if (currentStall != nullptr) {

				if (currentStall->isOpen && !currentStall->foodQueue.isFull() && foodStallMapList.checkFoodStallMapping(currentFood->id, currentStall->id)) {
					cout << currentStall->name << " stall is open and not busy. Assigning order to this stall." << endl;
					assigned = true;
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
				else if (!currentStall->isOpen || !foodStallMapList.checkFoodStallMapping(currentFood->id, currentStall->id)) {
					impossibleToAssignCount++;
				}
				else {
					cout << currentStall->name << " stall is busy. Moving to the next stall." << endl;
				}
			}
			else {
				cout << "No stall located." << endl;
			}
		}

		if (impossibleToAssignCount == stallCircularQueue.getMaxStalls()) {
			cout << "All stalls are either closed or cannot prepare this food item." << endl;
			cout << "Cannot assign this food item at this time." << endl;
			cout << "Food item will be removed from order." << endl;
			order->getFoodList()->getHead() = currentFood->next; // Remove the food item from the order
		}

		if (!assigned) {
			cout << "All stalls are either closed or busy." << endl;
			cout << "Cannot assign your food at this time." << endl;
			cout << "Order will remain in waiting queue as pending." << endl;
			break;
		}

		stallCircularQueue.displayQueue();
		currentFood = currentFood->next;
	}
};