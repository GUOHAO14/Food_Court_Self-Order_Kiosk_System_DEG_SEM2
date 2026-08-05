#include <iostream>
#include "globals.h"
#include "stall_assignment.h"

using namespace std;

void stallAndOrderAssignment(Order order) {

	struct food* currentFood = order.getFoodList()->getHead();

	// process each food item in the order
	for (int i = 0; i < order.getFoodList()->getCount(); i++) {
		if (currentFood == nullptr) {
			cout << "No food items in the order." << endl;
			break;
		}
		
		assignFoodToStall(currentFood);

		stallCircularQueue.displayQueue();
		currentFood = currentFood->next;
	}

	// refresh order status
	order.updateOrderStatus();
	order.displayOrder();

	if (order.getOrderStatus() == "Processing") {
		swapPendingToProcessingQueue(&order);
	}
};

void assignFoodToStall(food* currentFood) {
	// if all stalls are either closed or cannot prepare the food, count as impossible to assign
	// if all stalls impossible handle this food, this food will be dropped from order

	// queue status
	bool hasSuitableStall = false;
	bool assigned = false;
	int stallNum = stallCircularQueue.getMaxStalls();

	for (int k = 0; k < stallNum; k++) { // go through five stalls
		struct stall* currentStall = stallCircularQueue.getQueue()[stallCircularQueue.getFront() % stallNum];

		if (currentStall != nullptr) {
			// STALL ASSIGNMENT CIRCULAR QUEUE
			// then dequeue the stall in order assignment
			stallCircularQueue.dequeueStall();
			// re-enqueue the stall to the end of the circular queue 
			// to achieve rotate mechanism for load balancing
			stallCircularQueue.enqueueStall(currentStall);

			// stall not suitable for this food
			if (!foodStallMapList.checkFoodStallMapping(currentFood->id, currentStall->id)) {
				continue;
			}

			// suitable stall is open and free
			else if (currentStall->isOpen && !currentStall->foodQueue.isFull() && foodStallMapList.checkFoodStallMapping(currentFood->id, currentStall->id)) {

				hasSuitableStall = true;
				assigned = true;

				cout << currentStall->name << " stall is open and not busy. Assigning order to this stall." << endl;

				// change from Pending to Processing
				currentFood->status = "Processing";

				// assign order to the stall
				currentStall->foodQueue.enqueueFood(currentFood);

				// display the number of orders in the stall's queue
				cout << "This stall is currently preparing " << currentStall->foodQueue.getCount() << " orders." << endl;

				// display those specific orders
				currentStall->foodQueue.displayFoodQueue(); 

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
}

// check if unassigned food in the waiting queue can be assigned
// used when a stall mark food as done (free), or when a stall changes status to open
void checkAndAssignUnassignedFood() {
	if (unassignedFoodQueue.getCount() > 0) {
		cout << "Checking unassigned food items in the waiting queue..." << endl;
		struct food* currentFood = unassignedFoodQueue.getHead();
		while (currentFood != nullptr) {
			assignFoodToStall(currentFood);
			currentFood = currentFood->next;
		}
	}
	else {
		cout << "No unassigned food items in the waiting queue." << endl;
	}
}

void swapPendingToProcessingQueue(Order * order) {
	if (order->getOrderStatus() == "Processing") {
		Order temp = pendingOrdersQueue.delQueue();

		processingOrdersQueue.addQueue(temp);
	}
	else {
		cout << "Swap failed. Order status does not indicate Processing." << endl;
	}
}

void swapProcessingToCompletedQueue(Order* order) {
	if (order->getOrderStatus() == "Completed") {
		Order temp = processingOrdersQueue.delQueue();

		completedOrdersQueue.addQueue(temp);
	}
	else {
		cout << "Swap failed. Order status does not indicate Completed." << endl;
	}
}