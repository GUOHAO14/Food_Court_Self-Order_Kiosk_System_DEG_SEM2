#include "self_order_interface.h"
#include "globals.h"
#include <iostream>

using namespace std;

// must pass in student id
// isaac do

void printSelfOrderInt(int stuId) {
	// stall management interface
	int choice;
	do {
		cout << endl << "=============== Self-Order Kiosk ===============" << endl;
		cout << "Hello, you are student TP" << stuId << "." << endl;
		cout << "1. WIP" << endl;
		cout << "2. Display Stall Status" << endl;
		cout << "3. Back" << endl;

		cout << "Enter your choice (type integer): ";
		cin >> choice;

		switch (choice) {
		case 1:
			
			break;
		case 2:
			stallList.displayAllStalls();
			break;
		case 3:
			// Return to main menu
			break;
		default:
			cout << "Invalid input. Please try again." << endl;
		}

	} while (choice != 3);
}