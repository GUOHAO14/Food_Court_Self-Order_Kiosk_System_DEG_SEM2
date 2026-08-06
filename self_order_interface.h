#pragma once

#include "session.h"
#include "stall_assignment.h"

void printSelfOrderInt(int stuId);

// Kiosk Page Interfaces
void page1_MainHub(Session* session);
void page2_FoodMenu(Session* session);
void page3_CartReview(Session* session);
void page4_CheckoutConfirmation(Session* session);

// Action & Helper Flow Functions
void addItemToOrderFlow(Session* session);
void processFinalCheckout(Session* session);
bool performUndo(Session* session);
int getMaxOrderIdFromCSV(const char* filename = "order.csv");
void displayAllOrders();