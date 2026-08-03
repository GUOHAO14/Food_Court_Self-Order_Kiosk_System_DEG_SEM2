#pragma once
#include <iostream>

using namespace std;

// a struct to represent a food item
struct food {
    int id;
    string name;
    double price;
    int quantity;
	struct food* next; // pointer to next food item

    food(int id, string name, double price) {
        this->id = id;
        this->name = name;
        this->price = price;
        this->quantity = 0;
        this->next = nullptr;
    }
};

// a linked list to store chaining food items
class Food_Linked_List {
private:
    struct food* head;
    struct food* tail;
public:
    Food_Linked_List() {
        head = nullptr;
        tail = nullptr;
    }

    void insertRear(int id, string name, double price) {
        struct food* newFood = new food(id, name, price);

        if (head == nullptr) {
            head = newFood;
            tail = newFood;
        }
        else {
            tail->next = newFood;
            tail = newFood;
        }
    }
};
