#pragma once
#include <iostream>
#include "stall.h"

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

 //   void searchStallsByFoodId(struct stall *circularQueue[], int food_id) {
	//	struct food_stall_map* trav = head;

	//	for (trav != nullptr) {
	//		if (trav->food_id == food_id) {
	//			found = true;
	//		}
	//		trav = trav->next;
	//	}
	//	if (!found) {
	//		cout << "No stalls found for this food item." << endl;
	//	}
	//}

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
};