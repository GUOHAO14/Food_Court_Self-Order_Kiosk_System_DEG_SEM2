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
    stallList = Stall_Linked_List();
    foodList = Food_Linked_List();
    foodStallMapList = Food_Stall_Map_Linked_List();
    unassignedFoodQueue = Food_Linked_List();

    loadStallFromCSV(&stallList, "stall.csv");
    loadFoodFromCSV(&foodList, "food.csv");
    loadFoodStallMapFromCSV(&foodStallMapList, "food_stall_map.csv");

    do {
        cout << endl << "===== Campus Self-Order System =====" << endl;
        cout << "1. Enter Self-Order Kiosk" << endl;
        cout << "2. Stall Management Page" << endl;
        cout << "3. TESTING ORDER ASSIGNMENT" << endl;
        cout << "4. Exit Program" << endl;

        cout << "Enter your choice (type integer): ";
        cin >> choice;

        // remove bottom
        time_t now = time(nullptr);

        char buffer[26];

        ctime_s(buffer, sizeof(buffer), &now);

        string currentTime = buffer;
        Order* hello;
		struct food * selectedFood;
        //remove above
        switch (choice) {
		case 1:
			// enter order page, Isaac part
            printSelfOrderInt(76267);
			break;
        case 2: 
			chooseStall();
			break;
        case 3: 
            // remove bottom
            hello = new Order(1, 123456, currentTime, "Pending");
            selectedFood = foodList.searchFoodById(1);
            hello->addFood(food(selectedFood->id, selectedFood->name, selectedFood->price));

            // go stall assignment cpp
            stallAndOrderAssignment(hello);
            // remove above
            break;
        case 4: 
            return 0;
            break;
        default: 
			cout << "Invalid choice input. Please try again." << endl;
        }
    } while (choice != 4);
}