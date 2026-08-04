#include "stall_assignment.h"
#include "stall.h"
#include "queue.h"
#include "order_assignment_circular_queue.h"

Order_Assignment_Circular_Queue stallCircularQueue;

void stallAndOrderAssignment(Order* order) {
	stallCircularQueue = Order_Assignment_Circular_Queue(); 
	// Reset the circular queue for each new order to be assigned

	order.getFoodList().displayFood(); // Display the food items in the order
};