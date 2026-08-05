#pragma once
#include "session.h"
#include "stall_assignment.h"

void printSelfOrderInt(int stuId);
void createOrderInterface(Session* session);
void addItemToOrderFlow(Session* session);
void checkoutOrder(Session* session);
