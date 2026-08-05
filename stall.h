#pragma once
#include <iostream>
#include <iomanip>
#include "stall_internal_food_circular_queue.h"

using namespace std;

// a struct to represent a stall
struct stall {
    int id;
    string name;
	bool isOpen; // stall status is open / close
	bool isBusy; // stall status is busy / free (determined by order queue)
	struct stall* next; // pointer to next stall
	Stall_Internal_Food_Circular_Queue foodQueue;

    stall(int id, string name, bool isOpen) {
        this->id = id;
        this->name = name;
        this->isOpen = isOpen;
        this->isBusy = false;
        this->next = nullptr;
        this->foodQueue = Stall_Internal_Food_Circular_Queue();
    }
};

// a linked list to store chaining stalls
class Stall_Linked_List {
private:
    struct stall* head;
    struct stall* tail;
public:
    Stall_Linked_List() {
        head = nullptr;
        tail = nullptr;
    }

	// insert new stall (used during program initialisation)
    void insertRear(int id, string name, bool isOpen) {
        struct stall* newStall = new stall(id, name, isOpen);

        if (head == nullptr) {
            head = newStall;
            tail = newStall;
        }
        else {
            tail->next = newStall;
            tail = newStall;
        }
    }

	// search for a stall by its ID
	struct stall* searchStallById(int id) {
		struct stall* trav = head;

		while (trav != nullptr) {
			if (trav->id == id) {
				return trav;
			}
			trav = trav->next;
		}
		return nullptr; // return nullptr if stall not found
	}

	// used in stall_interface to set stall status
	void setStallStatus(struct stall* stall, bool isOpen) {
		bool currentStatus = stall->isOpen;

		if (currentStatus == isOpen) {
			cout << "Stall status is already " << (isOpen ? "Open" : "Closed") << "." << endl;
		}
		else {
			stall->isOpen = isOpen;
			cout << "Stall status has been changed to " << (isOpen ? "Open" : "Closed") << "." << endl;
		}
	}

	// header for stall display
	void displayStallsHeader(int length) {
		cout << string(length, '=') << endl;
		cout << "| ";
		cout << left << setw(3) << "No.";
		cout << " | ";
		cout << left << setw(8) << "Stall ID";
		cout << " | ";
		cout << left << setw(20) << "Stall Name";
		cout << " | ";
		cout << left << setw(10) << "Status";
		cout << " | ";
		cout << left << setw(12) << "Availability";
		cout << " |" << endl;
		cout << string(length, '=') << endl;
	}

	// actual display for stall data
	void displayStallsFormat(int count, struct stall* stall) {
		cout << "| ";
		cout << left << setw(3) << count;
		cout << " | ";
		cout << left << setw(8) << stall->id;
		cout << " | ";
		cout << left << setw(20) << stall->name;
		cout << " | ";
		cout << left << setw(10) << (stall->isOpen ? "Open" : "Closed");
		cout << " | ";
		cout << left << setw(12) << (stall->foodQueue.isFull() ? "Busy" : "Not Busy");
		cout << " |" << endl;
	}

	// display all stalls in a formatted table
	void displayAllStalls() {
		int length = 69;
		struct stall* trav = head;
		int count = 0;
		displayStallsHeader(length);
		while (trav != nullptr) {
			count++;
			displayStallsFormat(count, trav);
			trav = trav->next;
		}
		cout << string(length, '=') << endl << endl;
	}
};