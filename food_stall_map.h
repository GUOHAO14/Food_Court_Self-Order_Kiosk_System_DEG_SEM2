#pragma once
#include <iostream>

using namespace std;

// a struct to represent the mapping between food items and stalls
// this is because one food can be prepared by one or more stalls
// many-to-many relationship

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

// a linked list to store the mapping between food items and 

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
};