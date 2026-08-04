#pragma once
#include "stall.h"
#include "food.h"
#include "food_stall_map.h"

void chooseStall(Stall_Linked_List* stallListPtr, Food_Linked_List* foodListPtr, Food_Stall_Map_Linked_List* foodStallMapListPtr);
void printManageStallInt();
void printSetStallStatusInt();