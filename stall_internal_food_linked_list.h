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

class Stall_Internal_Food_Linked_List {
private:

    struct Node {
        foodForAssignment* data;
        Node* next;

        Node(foodForAssignment* foodOrder) {
            data = foodOrder;
            next = nullptr;
        }
    };

    Node* head;
    Node* tail;
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
    
    struct Node* getHead() {
        return head;
    }

    void insertRear(foodForAssignment* foodOrder) {

        if (isFull()) {
            cout << "Food queue is full." << endl;
            return;
        }

        Node* newNode = new Node(foodOrder);

        if (head == nullptr) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }

        count++;
    }

    bool markComplete(int displayIndex) {

        if (displayIndex < 0 || displayIndex >= count) {
            cout << "Invalid selection." << endl;
            return false;
        }

        Node* current = head;
        Node* previous = nullptr;

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

        Node* current = head;

        while (current != nullptr) {

            if (current->data->order->getOrderID() == orderId &&
                current->data->food->id == foodId) {
                return true;
            }

            current = current->next;
        }

        return false;
    }

    foodForAssignment* getFoodAssignment(int displayIndex) {

        if (displayIndex < 0 || displayIndex >= count)
            return nullptr;

        Node* current = head;

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

        Node* current = head;
        int count = 1;

        while (current != nullptr) {

            Food_For_Assignment_Linked_List().displayFoodAssignmentFormat(count,current->data);

            current = current->next;
            count++;
        }

        cout << string(length, '=') << endl << endl;
    }
};