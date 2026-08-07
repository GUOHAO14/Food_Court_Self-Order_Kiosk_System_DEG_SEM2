#pragma once
#include <iostream>
#include "food.h"
#include "food_stall_map.h"
#include "food_for_assignment.h"

using namespace std;

// this class is linked list (food waiting queue) for the stall itself
// max orders intake is 6. So 6 orders in queue indicate that the stall is busy
// this class also allows stall owner/staff to mark an order as complete
// hence dequeueing and freeing up the stall for new orders

struct foodForAssignNode {
    foodForAssignment* data;
    foodForAssignNode* next;

    foodForAssignNode(foodForAssignment* foodOrder) {
        data = foodOrder;
        next = nullptr;
    }
};

class Stall_Internal_Food_Linked_List {
private:

    foodForAssignNode* head;
    foodForAssignNode* tail;
    int count;
    int maxFoodOrder = 6;

public:

    Stall_Internal_Food_Linked_List() {
        head = nullptr;
        tail = nullptr;
        count = 0;
    }

    bool isFull() {
        return count >= maxFoodOrder;
    }

    bool isEmpty() {
        return head == nullptr;
    }

    int getCount() {
        return count;
    }

    int getMaxFoodOrder() {
        return maxFoodOrder;
    }
    
    struct foodForAssignNode* getHead() {
        return head;
    }

    void insertRear(foodForAssignment* foodOrder) {

        if (isFull()) {
            cout << "Food queue is full." << endl;
            return;
        }

        foodForAssignNode* newfoodForAssignNode = new foodForAssignNode(foodOrder);

        if (head == nullptr) {
            head = tail = newfoodForAssignNode;
        }
        else {
            tail->next = newfoodForAssignNode;
            tail = newfoodForAssignNode;
        }

        count++;
    }

    bool markComplete(int displayIndex) {

        if (displayIndex < 0 || displayIndex >= count) {
            cout << "Invalid selection." << endl;
            return false;
        }

        foodForAssignNode* current = head;
        foodForAssignNode* previous = nullptr;

        for (int i = 0; i < displayIndex; i++) {
            previous = current;
            current = current->next;
        }

        current->data->food->status = "Completed";

        // remove from linked list

        if (previous == nullptr) {
            head = current->next;
        }
        else {
            previous->next = current->next;
        }

        if (current == tail) {
            tail = previous;
        }

        delete current;

        count--;

        cout << "Food marked as completed." << endl;

        return true;
    }
    
    // check whether this stall is currently handling a specific food under an order
    bool containsFoodAssignment(int orderId, int foodId) {

        foodForAssignNode* current = head;

        while (current != nullptr) {

            if (current->data->order->getOrderID() == orderId &&
                current->data->food->id == foodId) {
                return true;
            }

            current = current->next;
        }

        return false;
    }

    int countFoodAssignment(int orderId, int foodId)
    {
        int count = 0;

        foodForAssignNode* current = head;

        while (current != nullptr)
        {
            if (current->data != nullptr &&
                current->data->order->getOrderID() == orderId &&
                current->data->food->id == foodId)
            {
                count++;
            }

            current = current->next;
        }

        return count;
    }

    foodForAssignment* getFoodAssignment(int displayIndex) {

        if (displayIndex < 0 || displayIndex >= count)
            return nullptr;

        foodForAssignNode* current = head;

        for (int i = 0; i < displayIndex; i++) {
            current = current->next;
        }

        return current->data;
    }

    void displayInternalFoodList() {

        if (isEmpty()) {
            cout << "No food orders assigned." << endl;
            return;
        }

        int length = 62;

        Food_For_Assignment_Linked_List().displayFoodAssignmentHeader(length);

        foodForAssignNode* current = head;
        int count = 1;

        while (current != nullptr) {

            Food_For_Assignment_Linked_List().displayFoodAssignmentFormat(count,current->data);

            current = current->next;
            count++;
        }

        cout << string(length, '=') << endl << endl;
    }
};