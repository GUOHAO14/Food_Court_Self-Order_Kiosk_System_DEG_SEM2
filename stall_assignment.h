#pragma once
#include "queue.h"
#include "stall.h"
#include "stall_assignment_circular_queue.h"

void stallAndOrderAssignment(Order order);
void assignFoodToStall(food* currentFood);
void checkAndAssignUnassignedFood();

void swapPendingToProcessingQueue(Order* order);
void swapProcessingToCompletedQueue(Order* order);
