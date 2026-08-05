#pragma once
#include <iostream>
#include "stall.h"
using namespace std;

// a struct to represent the mapping between food items and stalls
struct food_stall_map {
    int food_id;
    int stall_id;
    struct food_stall_map* next;

    food_stall_map(int food_id, int stall_id) {
        this->food_id = food_id;
        this->stall_id = stall_id;
        this->next = nullptr;
    }
};

// a linked list to store the mapping between food items and stalls
class Food_Stall_Map_Linked_List {
private:
    struct food_stall_map* head;
    struct food_stall_map* tail;

public:
    Food_Stall_Map_Linked_List() {
        head = nullptr;
        tail = nullptr;
    }

    void insertRear(int food_id, int stall_id) {
        struct food_stall_map* newMap = new food_stall_map(food_id, stall_id);

        if (head == nullptr) {
            head = newMap;
            tail = newMap;
        }
        else {
            tail->next = newMap;
            tail = newMap;
        }
    }

    bool checkFoodStallMapping(int food_id, int stall_id) {
        struct food_stall_map* trav = head;
        while (trav != nullptr) {
            if (trav->food_id == food_id && trav->stall_id == stall_id) {
                return true; // mapping exists
            }
            trav = trav->next;
        }
        return false; // mapping does not exist
    }

    // Remove mapping node when an item is deleted
    bool deleteMap(int food_id, int stall_id) {
        if (head == nullptr) return false;

        struct food_stall_map* current = head;
        struct food_stall_map* prev = nullptr;

        while (current != nullptr && !(current->food_id == food_id && current->stall_id == stall_id)) {
            prev = current;
            current = current->next;
        }

        if (current == nullptr) return false;

        if (current == head) {
            head = head->next;
            if (head == nullptr) tail = nullptr;
        }
        else {
            prev->next = current->next;
            if (current == tail) tail = prev;
        }

        delete current;
        return true;
    }
    bool isFoodUsedByAnyStall(int food_id) {
        struct food_stall_map* trav = head;
        while (trav != nullptr) {
            if (trav->food_id == food_id) {
                return true; // Found at least one stall still selling this
            }
            trav = trav->next;
        }
        return false; // No stall sells this food anymore
    }
};