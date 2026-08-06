#pragma once
#include "food.h"
#include "queue.h"   // order class

struct foodForAssignment
{
    Order* order;
    food* food;

    foodForAssignment* next;

    foodForAssignment(Order* order, struct food* food) {
        this->order = order;
        this->food = food;
        this->next = nullptr;
    }
};

// for general food pending queue (behaves like a normal queue)
// but some functions also used by STALL INTERNAL CIRCULAR QUEUE
class Food_For_Assignment_Linked_List {
private:
    foodForAssignment* head;
    foodForAssignment* tail;
    int count;

public:
    Food_For_Assignment_Linked_List() {
        head = nullptr;
        tail = nullptr;
        count = 0;
    }

    int getCount() {
        return count;
    }

    foodForAssignment* getHead() {
        return head;
    }

    void insertRear(Order* order, struct food* food) {
        struct foodForAssignment* newPair = new foodForAssignment(order, food);

        if (head == nullptr) {
            head = newPair;
            tail = newPair;
        }
        else {
            tail->next = newPair;
            tail = newPair;
        }

        count++;
    }

    void insertRear(struct foodForAssignment* foodOrder) {
        struct foodForAssignment* newPair = foodOrder;

        if (head == nullptr) {
            head = newPair;
            tail = newPair;
        }
        else {
            tail->next = newPair;
            tail = newPair;
        }

        count++;
    }

    Order* locateOrder(int foodId) {
        struct foodForAssignment* trav = head;

        while (trav != nullptr) {
            if (trav->food->id == foodId) {
                return trav->order;
            }
            trav = trav->next;
        }
        return nullptr;
    }

    void displayFoodAssignmentHeader(int length) {
        cout << string(length, '=') << endl;
        cout << "| ";
        cout << left << setw(3) << "No.";
        cout << " | ";
        cout << left << setw(8) << "Food ID";
        cout << " | ";
        cout << left << setw(30) << "Food Name";
        cout << " | ";
        cout << left << setw(8) << "Order ID";
        cout << " | ";
        cout << endl;
        cout << string(length, '=') << endl;
    }

    void displayFoodAssignmentFormat(int count, struct foodForAssignment* foodOrder) {
        cout << "| ";
        cout << left << setw(3) << count;
        cout << " | ";
        cout << left << setw(8) << foodOrder->food->id;
        cout << " | ";
        cout << left << setw(30) << foodOrder->food->name;
        cout << " | ";
        cout << left << setw(8) << foodOrder->order->getOrderID();
        cout << " |" << endl;
    }

    void displayAllFoodAssignment() {
        int length = 62;
        struct foodForAssignment* trav = head;
        int count = 0;
        displayFoodAssignmentHeader(length);
        while (trav != nullptr) {
            count++;
            displayFoodAssignmentFormat(count, trav);
            trav = trav->next;
        }
        cout << string(length, '=') << endl << endl;
    }
};
