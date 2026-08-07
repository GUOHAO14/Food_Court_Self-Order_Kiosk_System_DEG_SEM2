#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
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

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter an integer." << endl;

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice < 1 || choice > 6) {
            cout << "Invalid input. Please try again." << endl;
        }
        else if (choice == 6) {
            // go back
        }
        else {
            selectedStall = stallList.searchStallById(choice);
            
            if (selectedStall != nullptr) {

                string password;

                cout << "Enter password for " << selectedStall->name << " stall: ";

                cin >> password;

                if (verifyStallPassword(selectedStall->id, password, "stall_credentials.csv")) {
                    cout << "Login successful." << endl;

                    printManageStallInt();
                }
                else {
                    cout << "Incorrect password. Access denied." << endl;
                }
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
        cout << "2. Display All Stall Status" << endl;
        cout << "3. Display Processing Food Queue" << endl;
        cout << "4. Mark Food as Complete" << endl;
        cout << "5. Back" << endl;

        cout << "Enter your choice (type integer): ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter an integer." << endl;

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            printSetStallStatusInt();
            break;
        case 2:
            stallList.displayAllStalls();
            break;
        case 3:
            cout << "===== Food To Be Prepared =====" << endl;
            selectedStall->internalFoodList.displayInternalFoodList();
            break;
        case 4:
            markFoodAsComplete();
            break;
        case 5:
            break;
        default:
            cout << "Invalid input. Please try again." << endl;
        }

    } while (choice != 5);
}

void printSetStallStatusInt() {
    int choice;
    do {
        cout << endl << "===== Set Stall Status =====" << endl;
        cout << "1. Open Stall" << endl;
        cout << "2. Close Stall" << endl;
        cout << "3. Back" << endl;
        cout << "Enter your choice (type integer): ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter an integer." << endl;

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

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
    Stall_Internal_Food_Linked_List* internalFoodList = &selectedStall->internalFoodList;

    internalFoodList->displayInternalFoodList();
        
    cout << "Choose food to mark as complete. Enter Number (based on No. column). Enter to 0 to cancel: ";
    if (!(cin >> choice)) {
        cout << "Invalid input. Please enter an integer." << endl;

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (choice == 0) {
        cout << "Action cancelled." << endl;
        return;
    }

    if (choice < 1 || choice > internalFoodList->getCount()) {
        cout << "Invalid selection. Please choose a valid food number." << endl;
        return;
    }

    int index = choice - 1;
    // locate order based on food ID
    foodForAssignment* assignment = internalFoodList->getFoodAssignment(index);

    if (assignment == nullptr) {
        cout << "Food assignment not found." << endl;
        return;
    }

    if (internalFoodList->markComplete(index)) {

        Order* order = assignment->order;

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
    stallAndOrderAssignment();
}