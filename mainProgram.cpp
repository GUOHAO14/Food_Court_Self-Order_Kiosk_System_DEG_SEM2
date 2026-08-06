#include <iostream>
#include <ctime>
#include "globals.h"
#include "stall.h"
#include "food.h"
#include "food_stall_map.h"
#include "utils.h"
#include "stall_interface.h"
#include "stall_assignment.h"
#include "self_order_interface.h"
#include "stall_assignment_circular_queue.h"
#include "admin.h"
#include "student_auth.h"

using namespace std;

int main(){
    int choice;
    //Queue pending;
    //Queue completed;

    //pending.addQueue(Order("O001", "TP076357", "Chicken Rice"));
    //pending.addQueue(Order("O002", "TP076358", "Nasi Lemak"));
    //pending.addQueue(Order("O003", "TP076359", "Burger"));

    //cout << "Pending Queue:\n";
    //pending.displayQueue();

    //cout << "\nQueue Size: " << pending.queueNum() << endl;

    //cout << "\nDequeue one order...\n";
    //completed.addQueue(pending.delQueue());
    //pending.displayQueue();
    //cout << "\nCompleted Queue:\n";
    //completed.displayQueue();

    // INITIALISATION OF GLOBAL VARIABLES
    // USABLE BY ANY FILES, SINCE ALREADY DECLARED IN globals.h file
    stallList = Stall_Linked_List();
    foodList = Food_Linked_List();
    foodStallMapList = Food_Stall_Map_Linked_List();
    //unassignedFoodQueue = Food_Linked_List();
    stallCircularQueue = Stall_Assignment_Circular_Queue();

    pendingOrdersQueue = Queue();
    processingOrdersQueue = Queue();
    completedOrdersQueue = Queue();

    loadStallFromCSV(&stallList, "stall.csv");
    loadFoodFromCSV(&foodList, "food.csv");
    loadFoodStallMapFromCSV(&foodStallMapList, "food_stall_map.csv");

    do {
        cout << endl << "===== Campus Self-Order System =====" << endl;
        cout << "1. Enter Self-Order Kiosk" << endl;
        cout << "2. Stall Management Page" << endl;
        cout << "3. Admin Page" << endl;
        cout << "5. Exit Program" << endl;

        cout << "Enter your choice (type integer): ";
        cin >> choice;

        switch (choice) {
		case 1: {
            // enter order page, Isaac part
            int studentId = studentAuthMenu();

            if (studentId != -1) {
                printSelfOrderInt(studentId);
            }
            break;
        }
        case 2: 
			chooseStall();
			break;
        case 3:
            adminPage();
            break;
        case 4: 
            return 0;
            break;
        default: 
			cout << "Invalid choice input. Please try again." << endl;
        }
    } while (choice != 5);
}