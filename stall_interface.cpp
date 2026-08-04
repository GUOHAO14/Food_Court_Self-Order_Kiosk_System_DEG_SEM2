#include <iostream>
#include <iomanip>
#include "stall_interface.h"

using namespace std;

// global variables
Stall_Linked_List stallList;
Food_Linked_List foodList;
Food_Stall_Map_Linked_List foodStallMapList;
struct stall* selectedStall = nullptr;

void chooseStall(Stall_Linked_List* stallListPtr, Food_Linked_List* foodListPtr, Food_Stall_Map_Linked_List* foodStallMapListPtr) {
	// function to identify which stall
	int choice;
	stallList = *stallListPtr;
	foodList = *foodListPtr;
	foodStallMapList = *foodStallMapListPtr;

	bool endLoop = false;
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
			cout << "Returning to main menu." << endl;
		}
		else {
			selectedStall = stallList.searchStallById(choice);
			if (selectedStall != nullptr) {
				endLoop = true;
				cout << "You have chosen stall number " << selectedStall->id << "." << endl;
				printManageStallInt();
			}
			else {
				cout << "Stall not found." << endl;
			}
		}
	} while (endLoop);
}

void printManageStallInt() {
	// stall management interface
	int choice;
	do {
		cout << endl << "=============== Stall Management ===============" << endl;
		cout << "Hello, you are stall " << selectedStall->name << " (ID: " << selectedStall->id << ")." << endl;
		cout << "1. Set Stall Status" << endl;
		cout << "2. Manage Order Status" << endl;
		cout << "3. Back" << endl;

		cout << "Enter your choice (type integer): ";
		cin >> choice;

		switch (choice) {
		case 1:
			printSetStallStatusInt();
			break;
		case 2:
			// Call function to manage order status
			break;
		case 3:
			// Return to main menu
			break;
		default: 
			cout << "Invalid input. Please try again." << endl;
		}

	} while (choice < 1 || choice > 3);
}

void printSetStallStatusInt() {
	// to let stalls set open or close status
	int choice;
	do {
		cout << endl << "=============== Set Stall Status ===============" << endl;
		cout << "1. Open Stall" << endl;
		cout << "2. Close Stall" << endl;
		cout << "3. Back" << endl;
		cout << "Enter your choice (1 for Open, 2 for Close): ";
		cin >> choice;

		switch (choice) {
		case 1:
			stallList.setStallStatus(selectedStall, true);
			break;
		case 2:
			stallList.setStallStatus(selectedStall, false);
			break;
		case 3:
			// Return to main menu
			break;
		default:
			cout << "Invalid input. Please try again." << endl;
		}

	} while (choice < 1 || choice > 3);
}