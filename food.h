#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <algorithm> // for transform/equal
#include <cctype>    // for tolower

using namespace std;

// Forward declaration of Food_Stall_Map_Linked_List so it can be passed as a reference
class Food_Stall_Map_Linked_List;

// Cross-platform case-insensitive string comparison helper
inline bool isEqualIgnoreCase(const string& str1, const string& str2) {
    if (str1.length() != str2.length()) return false;
    return equal(str1.begin(), str1.end(), str2.begin(), [](char a, char b) {
        return tolower(static_cast<unsigned char>(a)) == tolower(static_cast<unsigned char>(b));
        });
}

// Struct to represent a food item
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

    food(int id, string name, double price, int quantity) {
        this->id = id;
        this->name = name;
        this->price = price;
        this->quantity = quantity;
        this->next = nullptr;
    }
};

// Linked list to store chaining food items
class Food_Linked_List {
private:
    struct food* head;
    struct food* tail;
    int count;

public:
    Food_Linked_List() {
        head = nullptr;
        tail = nullptr;
        count = 0;
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

        count++;
    }

    void insertRear(int id, string name, double price, int quantity) {
        struct food* newFood = new food(id, name, price, quantity);

        if (head == nullptr) {
            head = newFood;
            tail = newFood;
        }
        else {
            tail->next = newFood;
            tail = newFood;
        }

        count++;
    }

    void insertRear(food Food) {
        struct food* newFood = new food(Food.id, Food.name, Food.price);

        if (head == nullptr) {
            head = newFood;
            tail = newFood;
        }
        else {
            tail->next = newFood;
            tail = newFood;
        }

        count++;
    }

    int getCount() {
        return count;
    }

    struct food* getHead() {
        return head;
    }

    struct food* getTail() {
        return tail;
    }

    // Search for a food item by its ID
    struct food* searchFoodById(int id) {
        struct food* trav = head;

        while (trav != nullptr) {
            if (trav->id == id) {
                return trav;
            }
            trav = trav->next;
        }
        return nullptr;
    }

    // Header for food display
    void displayFoodHeader(bool withQuantity, int length) {
        cout << string(length, '=') << endl;
        cout << "| ";
        cout << left << setw(3) << "No.";
        cout << " | ";
        cout << left << setw(8) << "Food ID";
        cout << " | ";
        cout << left << setw(30) << "Food Name";
        cout << " | ";
        if (withQuantity) {
            cout << left << setw(8) << "Quantity";
            cout << " | ";
        }
        cout << left << setw(10) << "Price (RM)";
        cout << " | ";
        cout << endl;
        cout << string(length, '=') << endl;
    }

    // Actual display for food data
    void displayFoodFormat(int count, struct food* food, bool withQuantity) {
        cout << "| ";
        cout << left << setw(3) << count;
        cout << " | ";
        cout << left << setw(8) << food->id;
        cout << " | ";
        cout << left << setw(30) << food->name;
        cout << " | ";
        if (withQuantity) {
            cout << left << setw(8) << food->quantity;
            cout << " | ";
            cout << left << setw(10) << food->price * food->quantity;
            cout << " |" << endl;
        }
        else {
            cout << left << setw(10) << food->price;
            cout << " |" << endl;
        }
    }

    // Display all food in a formatted table
    void displayAllFood(bool withQuantity) {
        int length = 64;
        if (withQuantity) {
            length = 75;
        }
        struct food* trav = head;
        int count = 0;
        displayFoodHeader(withQuantity, length);
        while (trav != nullptr) {
            count++;
            displayFoodFormat(count, trav, withQuantity);
            trav = trav->next;
        }
        cout << string(length, '=') << endl << endl;
    }

    // --- MEMBER METHOD DECLARATIONS ---
    // (Implementations are in stall_interface.cpp where full Food_Stall_Map_Linked_List definition exists)
    int displayFoodByStall(int stallId, Food_Stall_Map_Linked_List& mapList);
    int displayAvailableExistingFood(int currentStallId, Food_Stall_Map_Linked_List& mapList);

    void saveOrderMapFood(ofstream& out, int orderID) {
        food* current = head;
        while (current != nullptr) {
            out << orderID << "," << current->id << endl;
            current = current->next;
        }
    }

    // Deletes a food item by ID and keeps head, tail, and count properly synchronized
    bool deleteFoodById(int id) {
        if (head == nullptr) {
            return false;
        }

        struct food* current = head;
        struct food* prev = nullptr;

        while (current != nullptr && current->id != id) {
            prev = current;
            current = current->next;
        }

        if (current == nullptr) {
            return false;
        }

        if (current == head) {
            head = head->next;
            if (head == nullptr) {
                tail = nullptr;
            }
        }
        else {
            prev->next = current->next;
            if (current == tail) {
                tail = prev;
            }
        }

        delete current;
        count--;
        return true;
    }

    // Checks whether a food name already exists in the global list (case-insensitive)
    bool isFoodNameExists(const string& name) {
        struct food* trav = head;
        while (trav != nullptr) {
            if (isEqualIgnoreCase(trav->name, name)) {
                return true;
            }
            trav = trav->next;
        }
        return false;
    }
};