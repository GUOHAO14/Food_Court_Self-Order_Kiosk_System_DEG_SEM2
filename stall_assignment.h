#pragma once
#include "queue.h"
#include "stall.h"
#include "utils.h"
#include "stall_assignment_circular_queue.h"
#include "food_for_assignment.h"

void stallAndOrderAssignment();
void assignFoodToStall(foodForAssignment* soloFoodOrder);
void iterateOrderFoodList(Order* order);

void swapPendingToProcessingQueue(Order* order);
void swapProcessingToCompletedQueue(Order* order);
