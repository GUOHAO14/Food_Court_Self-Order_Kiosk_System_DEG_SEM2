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
void loadOrderFromCSV(Queue* pending, Queue* completed, Food_Linked_List* foodList, string orderFile, string mapfoodFile);
void saveOrderToCSV(Queue* pending, Queue* completed, string orderFile, string mapfoodFile);