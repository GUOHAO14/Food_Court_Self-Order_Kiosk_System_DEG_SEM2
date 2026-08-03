#pragma once
#include <iostream>

using namespace std;

// a struct to represent a stall
struct stall {
    int id;
    string name;
	struct stall* next; // pointer to next stall

    stall(int id, string name) {
        this->id = id;
        this->name = name;
        this->next = nullptr;
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

    void insertRear(int id, string name) {
        struct stall* newStall = new stall(id, name);

        if (head == nullptr) {
            head = newStall;
            tail = newStall;
        }
        else {
            tail->next = newStall;
            tail = newStall;
        }
    }
};