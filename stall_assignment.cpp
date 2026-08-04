#include <iostream>
#include "stall_assignment.h"

Order_Assignment_Circular_Queue stallCircularQueue;

void stallAndOrderAssignment(Order * order) {
	stallCircularQueue = Order_Assignment_Circular_Queue(); 
	// Reset the circular queue for each new order to be assigned

	//order->getFoodList().displayAllFood(); // Display the food items in the order

};