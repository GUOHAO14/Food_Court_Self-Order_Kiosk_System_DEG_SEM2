#include <iostream>
#include "globals.h"
#include "stall_assignment.h"

using namespace std;

// check pending queue and assign food to stalls
void stallAndOrderAssignment() {

	Node* firstPendingOrder = pendingOrdersQueue.getHead();

	while (firstPendingOrder != nullptr) {

		// save next node first
		Node* nextOrderNode = firstPendingOrder->next;

		Order* order = firstPendingOrder->data;

		iterateOrderFoodList(order);

		cout << endl;
		order->displayOrder();

		// move using saved pointer
		firstPendingOrder = nextOrderNode;
	}
}

// process each food item in the order
void iterateOrderFoodList(Order* order) {
	struct food* currentFood = order->getFoodList()->getHead();

	if (currentFood == nullptr) {
		cout << "No food items in the order." << endl;
		return;
	}

	while (currentFood != nullptr) {

		// business logic: one order can have many food
		// EACH FOOD IN AN ORDER is allocated to stalls EVENLY
		// which means: one order could be handled by multiple stalls
		// ORDER STATUS is affected by FOOD STATUS
		// status consists of Pending (in waiting queue), Processing (food/all food assigned to stall), Completed (all food marked as complete) 

		// each food in an order is handled one by one
		// then food status will also update order status

		if (currentFood->status == "Pending") {
			struct foodForAssignment* newSoloFoodOrder = new foodForAssignment(order, currentFood);
			assignFoodToStall(newSoloFoodOrder);
		}
		currentFood = currentFood->next;
	}

	// update order status
	order->updateOrderStatus();

	if (order->getOrderStatus() == "Processing") {
		swapPendingToProcessingQueue(order);
	}

	// save new update
	saveOrderToCSV(&pendingOrdersQueue, &processingOrdersQueue, &completedOrdersQueue, &stallList, "order.csv", "order_map_food.csv");
}

// MAIN STALL CIRCULAR QUEUE ASSIGNMENT LOGIC
// handle one food
void assignFoodToStall(foodForAssignment* soloFoodOrder) {
	// if all stalls are either closed or cannot prepare the food, count as impossible to assign
	// if all stalls impossible handle this food, this food will be dropped from order

	food* currentFood = soloFoodOrder->food;
	Order* currentOrder = soloFoodOrder->order;

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
			else if (currentStall->isOpen && !currentStall->internalFoodList.isFull() && foodStallMapList.checkFoodStallMapping(currentFood->id, currentStall->id)) {

				hasSuitableStall = true;
				assigned = true;

				cout << currentStall->name << " stall is open and not busy. Assigning order to this stall." << endl;

				// change from Pending to Processing
				currentFood->status = "Processing";

				// assign order to the stall
				currentStall->internalFoodList.insertRear(soloFoodOrder);

				// display the number of orders in the stall's queue
				cout << "This stall is currently preparing " << currentStall->internalFoodList.getCount() << " orders." << endl;

				break; // Exit after assigning the order
			}

			// suitable stall that is open but not free, or is closed
			else if (foodStallMapList.checkFoodStallMapping(currentFood->id, currentStall->id) && ((currentStall->isOpen && currentStall->internalFoodList.isFull()) || !currentStall->isOpen)) {

				hasSuitableStall = true;
				assigned = false;

				cout << currentStall->name << " stall is busy or closed. Order will remain in Pending Queue." << endl;

				//unassignedFoodQueue.insertRear(soloFoodOrder);
			}

			else {
				cout << "An error occured, stall is skipped. Moving to the next stall." << endl;
			}
		}
		else {
			cout << "No stall located." << endl;
		}
	}

	if (!hasSuitableStall && !assigned) {
		cout << "An error occured, your food order will be dropped" << endl;
		pendingOrdersQueue.delQueue();
		delete soloFoodOrder;
	}
}

void swapPendingToProcessingQueue(Order * order) {
	if (order->getOrderStatus() == "Processing") {
		Order* temp = pendingOrdersQueue.delQueue();

		processingOrdersQueue.addQueue(temp);

		// save new update
		saveOrderToCSV(&pendingOrdersQueue, &processingOrdersQueue, &completedOrdersQueue, &stallList, "order.csv", "order_map_food.csv");
	}
	else {
		cout << "Swap failed. Order status does not indicate Processing." << endl;
	}
}

void swapProcessingToCompletedQueue(Order* order) {
	if (order->getOrderStatus() == "Completed") {
		Order* temp = processingOrdersQueue.delQueue();

		completedOrdersQueue.addQueue(temp);

		// save new update
		saveOrderToCSV(&pendingOrdersQueue, &processingOrdersQueue, &completedOrdersQueue, &stallList, "order.csv", "order_map_food.csv");
	}
	else {
		cout << "Swap failed. Order status does not indicate Completed." << endl;
	}
}