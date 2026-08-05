#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H
#include "food.h"
#include "stall.h"
#include "food_stall_map.h"
#include "stall_assignment_circular_queue.h"
#include "queue.h"

// 'inline' keyword allows variables to be defined in multiple 
// translation units without violating the One Definition Rule (ODR).

inline Stall_Linked_List stallList;
inline Food_Linked_List foodList;
inline Food_Stall_Map_Linked_List foodStallMapList;
inline Food_Linked_List unassignedFoodQueue;
inline Stall_Assignment_Circular_Queue stallCircularQueue;

inline static Queue pendingOrdersQueue;
inline static Queue processingOrdersQueue;
inline static Queue completedOrdersQueue;

#endif