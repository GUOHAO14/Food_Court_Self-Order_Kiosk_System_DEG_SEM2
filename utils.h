#pragma once
#include "food.h"
#include "stall.h"
#include "food_stall_map.h"
#include "queue.h"
#include "stall_assignment_circular_queue.h"
#include "globals.h"

// define function prototypes
void loadFoodFromCSV(Food_Linked_List* food, string fileName);
void loadStallFromCSV(Stall_Linked_List* stall, string fileName);
void loadFoodStallMapFromCSV(Food_Stall_Map_Linked_List* map, string fileName);
void loadOrderFromCSV(Queue* pending, Queue* processing, Queue* completed, Food_Linked_List* foodList, string orderFile, string mapfoodFile);
void saveOrderToCSV(Queue* pending, Queue* processing, Queue* completed, Stall_Linked_List* stallList, string orderFile, string mapfoodFile);
void saveOrders(ofstream& orderOut, ofstream& mapOut, Queue* queue, Stall_Linked_List* stallList = nullptr);
void saveOrderMapFood(ofstream& out, int orderId, Order* order, Stall_Linked_List* stallList = nullptr);
void saveStallCircularQueue(Stall_Linked_List* stallList, string fileName);
