#pragma once
#include <iostream>
#include <iomanip>
#include "stall_internal_food_linked_list.h"

using namespace std;

// a struct to represent a stall
struct stall {
    int id;
    string name;
    bool isOpen; // stall status is open / close
    bool isBusy; // stall status is busy / free (determined by order queue)
    struct stall* next; // pointer to next stall
    Stall_Internal_Food_Linked_List internalFoodList;

    stall(int id, string name, bool isOpen) {
        this->id = id;
        this->name = name;
        this->isOpen = isOpen;
        this->isBusy = false;
        this->next = nullptr;
        this->internalFoodList = Stall_Internal_Food_Linked_List();
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

    // Getters for linked list pointers
    struct stall* getHead() {
        return head;
    }

    struct stall* getTail() {
        return tail;
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
        cout << left << setw(17) << "Availability";
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
        string text = to_string(stall->internalFoodList.getCount()) + " / " + to_string(stall->internalFoodList.getMaxFoodOrder()) + " (" + (stall->internalFoodList.isFull() ? "Busy" : "Not Busy") + ")";
        cout << left << setw(17) << text ;
        cout << " |" << endl;
    }

    // display all stalls in a formatted table
    void displayAllStalls() {
        cout << endl << "===== Stall Availability =====" << endl;
        int length = 74;
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

    void displayAllStallAssignment() {
        struct stall* stall = head;

        cout << endl;

        while (stall != nullptr) {

            cout << "=====" << stall->name << "=====" << endl;

            stall->internalFoodList.displayInternalFoodList();

            cout << endl;

            stall = stall->next;
        }
    }
};