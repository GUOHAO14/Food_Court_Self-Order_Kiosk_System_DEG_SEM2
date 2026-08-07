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

		bool removed = pendingOrdersQueue.delQueue(order->getOrderID());

		if (removed) {
			// transfer order to completed orders queue
			processingOrdersQueue.addQueue(order);

			// save new update
			saveOrderToCSV(&pendingOrdersQueue, &processingOrdersQueue, &completedOrdersQueue, &stallList, "order.csv", "order_map_food.csv");
		}
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

void displayOrderStallAssignment(Order* order) {
	if (order == nullptr)
	{
		cout << "Invalid order." << endl;
		return;
	}

	int orderId = order->getOrderID();

	cout << endl;
	cout << "====== Order Stall Assignment =====" << endl;
	cout << "Order ID: " << orderId << endl;
	cout << "Order Time: " << order->getOrderTime() << endl;
	cout << "Order Status: " << order->getOrderStatus() << endl;
	cout << endl;

	cout << string(75, '=') << endl;

	cout << "| ";
	cout << left << setw(10) << "Food ID";
	cout << " | ";
	cout << setw(25) << "Food Name";
	cout << " | ";
	cout << setw(10) << "Price";
	cout << " | ";
	cout << setw(15) << "Status";
	cout << " | ";
	cout << setw(15) << "Assigned Stall";
	cout << " |" << endl;


	cout << string(75, '-') << endl;

	food* currentFood = order->getFoodList()->getHead();

	while (currentFood != nullptr) {

		cout << "| ";
		cout << left << setw(10) << currentFood->id;
		cout << " | ";
		cout << setw(25) << currentFood->name;
		cout << " | ";
		cout << setw(10) << currentFood->price;
		cout << " | ";
		cout << setw(15) << currentFood->status;

		if (currentFood->status != "Processing") {
			cout << " | ";
			cout << setw(15) << "N/A";
			cout << " |" << endl;
		}
		else {
			string assignedStall = "Unknown";

			stall* currentStall = stallList.getHead();

			while (currentStall != nullptr)
			{
				foodForAssignNode* currentAssignment =
					currentStall->internalFoodList.getHead();

				while (currentAssignment != nullptr)
				{
					foodForAssignment* assignment =
						currentAssignment->data;

					if (assignment->order == order &&
						assignment->food == currentFood)
					{
						assignedStall = currentStall->name;
						break;
					}

					currentAssignment = currentAssignment->next;
				}

				if (assignedStall != "Unknown")
					break;

				currentStall = currentStall->next;
			}

			cout << " | ";
			cout << setw(15) << assignedStall;
			cout << " |" << endl;
		}
		currentFood = currentFood->next;
	}
	cout << string(75, '=') << endl;
}


void displayStudentOrderStallAssignment(int studentId) {
	cout << endl;
	cout << "===== Student Order Stall Assignment =====" << endl;
	cout << "Student ID: " << studentId << endl;


	// pending orders
	cout << endl << "===== Pending Orders =====" << endl;

	Node* currentPending = pendingOrdersQueue.getHead();

	while (currentPending != nullptr)
	{
		Order* order = currentPending->data;

		if (order->getStudentID() == studentId)
		{
			displayOrderStallAssignment(order);
		}

		currentPending = currentPending->next;
	}



	// processing orders
	cout << endl << "===== Processing Orders =====" << endl;

	Node* currentProcessing = processingOrdersQueue.getHead();

	while (currentProcessing != nullptr)
	{
		Order* order = currentProcessing->data;

		if (order->getStudentID() == studentId)
		{
			displayOrderStallAssignment(order);
		}

		currentProcessing = currentProcessing->next;
	}



	// completed orders
	cout << endl << "===== Completed Orders =====" << endl;

	Node* currentCompleted = completedOrdersQueue.getHead();

	while (currentCompleted != nullptr)
	{
		Order* order = currentCompleted->data;

		if (order->getStudentID() == studentId)
		{
			displayOrderStallAssignment(order);
		}

		currentCompleted = currentCompleted->next;
	}
}