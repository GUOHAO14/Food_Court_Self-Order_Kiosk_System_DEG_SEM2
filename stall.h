#pragma once
#include <iostream>

using namespace std;

struct stall {
    int id;
    string name;
    struct stall* next;

    stall(int id, string name) {
        this->id = id;
        this->name = name;
        this->next = nullptr;
    }
};


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