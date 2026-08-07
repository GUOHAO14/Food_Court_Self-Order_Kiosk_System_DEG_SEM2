#include <iostream>
#include <iomanip>
#include <string>
#include "globals.h"
#include "stall_interface.h"

using namespace std;

// Global stall selection pointer
struct stall* selectedStall = nullptr;

// Displays ONLY food items that belong to the specified stall ID
int Food_Linked_List::displayFoodByStall(int stallId, Food_Stall_Map_Linked_List& mapList) {
    int length = 64;
    struct food* trav = head;
    int displayCount = 0;

    displayFoodHeader(false, length);

    while (trav != nullptr) {
        if (mapList.checkFoodStallMapping(trav->id, stallId)) {
            displayCount++;
            displayFoodFormat(displayCount, trav, false);
        }
        trav = trav->next;
    }

    cout << string(length, '=') << endl << endl;
    return displayCount;
}

// Displays ONLY food items that are NOT currently sold by the specified stall ID
int Food_Linked_List::displayAvailableExistingFood(int currentStallId, Food_Stall_Map_Linked_List& mapList) {
    int length = 64;
    struct food* trav = head;
    int count = 0;

    displayFoodHeader(false, length);

    while (trav != nullptr) {
        if (!mapList.checkFoodStallMapping(trav->id, currentStallId)) {
            count++;
            displayFoodFormat(count, trav, false);
        }
        trav = trav->next;
    }

    cout << string(length, '=') << endl << endl;
    return count;
}

void chooseStall() {
    int choice;

    do {
        cout << endl << "=============== Choose Stall ===============" << endl;
        cout << "1. Western" << endl;
        cout << "2. Asian" << endl;
        cout << "3. Chinese" << endl;
        cout << "4. Mamak" << endl;
        cout << "5. Indian" << endl;
        cout << "6. Back " << endl;

        cout << "Enter your choice (type integer): ";
        cin >> choice;

        if (choice < 1 || choice > 6) {
            cout << "Invalid input. Please try again." << endl;
        }
        else if (choice == 6) {
            // Go back
        }
        else {
            selectedStall = stallList.searchStallById(choice);
            if (selectedStall != nullptr) {
                printManageStallInt();
            }
            else {
                cout << "Stall not found." << endl;
            }
        }
    } while (choice != 6);
}

void printManageStallInt() {
    int choice;
    do {
        cout << endl << "=============== Stall Management ===============" << endl;
        cout << "Hello, you are stall " << selectedStall->name << " (ID: " << selectedStall->id << ")." << endl;
        cout << "1. Set Stall Status" << endl;
        cout << "2. Display Processing Food Queue" << endl;
        cout << "3. Mark Food as Complete" << endl;
        cout << "4. Back" << endl;

        cout << "Enter your choice (type integer): ";
        cin >> choice;

        switch (choice) {
        case 1:
            printSetStallStatusInt();
            break;
        case 2:
            cout << "===== Food To Be Prepared =====" << endl;
            selectedStall->foodQueue.displayFoodQueue();
            break;
        case 3:
            markFoodAsComplete();
            break;
        case 4:
            break;
        default:
            cout << "Invalid input. Please try again." << endl;
        }

    } while (choice != 4);
}

void printSetStallStatusInt() {
    int choice;
    do {
        cout << endl << "===== Set Stall Status =====" << endl;
        cout << "1. Open Stall" << endl;
        cout << "2. Close Stall" << endl;
        cout << "3. Back" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            stallList.setStallStatus(selectedStall, true);

            // check pending queue again to be allocated to processing queue
            // if possible
            stallAndOrderAssignment();

            break;
        case 2:
            stallList.setStallStatus(selectedStall, false);
            break;
        case 3:
            break;
        default:
            cout << "Invalid input. Please try again." << endl;
        }

    } while (choice != 3);
}

void markFoodAsComplete() {
    int choice;
    cout << endl << "===== Mark Food As Complete =====" << endl;
    Stall_Internal_Food_Circular_Queue* foodQueue = &selectedStall->foodQueue;
    foodQueue->displayFoodQueue();
        
    cout << "Choose food to mark as complete. Enter Number (based on No. column): ";
    cin >> choice;
    int index = choice - 1;
    // locate order based on food ID
    Order* order = foodQueue->getQueue()[index]->order;

    if (foodQueue->markComplete(index)) {

        order->updateOrderStatus();

        if (order->getOrderStatus() == "Completed") {
            // dequeue from processing orders queue
            bool removed = processingOrdersQueue.delQueue(order->getOrderID());

            if (removed) {
                // transfer order to completed orders queue
                completedOrdersQueue.addQueue(order);

                // save new update
                saveOrderToCSV(&pendingOrdersQueue, &processingOrdersQueue, &completedOrdersQueue, &stallList, "order.csv", "order_map_food.csv");
            }
        }
    }

    // check pending queue again to be allocated to processing queue
    // if possible
    stallAndOrderAssignment();
}
