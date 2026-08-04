#pragma once
#include "food.h"
#include "stall.h"
#include "food_stall_map.h"
#include <iostream>

// define function prototypes
void loadFoodFromCSV(Food_Linked_List* food, string fileName);
void loadStallFromCSV(Stall_Linked_List* stall, string fileName);
void loadFoodStallMapFromCSV(Food_Stall_Map_Linked_List* map, string fileName);
