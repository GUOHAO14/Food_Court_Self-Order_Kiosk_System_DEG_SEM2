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

// a struct to represent a food item
struct food {
    int id;
    string name;
    double price;
    string status;  
    struct food* next; // pointer to next food item

    food(int id, string name, double price) {
        this->id = id;
        this->name = name;
        this->price = price;
        this->status = "Pending";
        this->next = nullptr;
    }

    food(int id, string name, double price, string status) {
        this->id = id;
        this->name = name;
        this->price = price;
        this->status = status;
        this->next = nullptr;
    }
};

// a linked list to store chaining food items
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

    void insertRear(int id, string name, double price, string status) {
        struct food* newFood = new food(id, name, price, status);

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
        struct food* newFood = new food(Food.id, Food.name, Food.price, Food.status);

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

    void insertRear(food* newFood) {
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


    void deleteById(int id) {
        if (head == nullptr) {
            cout << "Cannot delete from an empty list." << endl;
            return;
        }

        struct food* prev = nullptr;
        struct food* deleteNode = head;

        while (deleteNode != nullptr) {
            if (deleteNode->id == id) {
                break;
            }
            prev = deleteNode;
            deleteNode = deleteNode->next;
        }

        if (deleteNode == nullptr) {
            cout << "Food ID \"" << id << "\" is not found." << endl;
        }
        else {
            if (deleteNode == head) {
                head = head->next;
                if (deleteNode == tail) {
                    tail = nullptr;
                }
            }
            else if (deleteNode == tail) {
                tail = prev;
                tail->next = nullptr;
            }
            else {
                prev->next = deleteNode->next;
            }
            count--;
            delete deleteNode;
        }
    }

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


    void detachFront() {
        if (head == nullptr) {
            cout << "Cannot delete from an empty list." << endl;
            return;
        }

        struct food* deleteNode = head;

        if (head->next == nullptr) {
            head = tail = nullptr;
        }
        else {
            head = head->next;
            deleteNode->next = nullptr;
        }
        count--;
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

    // Search for food by name (case-insensitive)
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


    // Header display
    void displayFoodHeader(int length) {
        cout << string(length, '=') << endl;
        cout << "| ";
        cout << left << setw(3) << "No.";
        cout << " | ";
        cout << left << setw(8) << "Food ID";
        cout << " | ";
        cout << left << setw(30) << "Food Name";
        cout << " | ";
        cout << left << setw(10) << "Price (RM)";
        cout << " | ";
        cout << endl;
        cout << string(length, '=') << endl;
    }

    // Overload for backward compatibility with stall_interface.cpp (ignores flag parameter)
    void displayFoodHeader(bool /*unused*/, int length) {
        displayFoodHeader(length);
    }

    // Row formatting
    void displayFoodFormat(int count, struct food* food) {
        cout << "| ";
        cout << left << setw(3) << count;
        cout << " | ";
        cout << left << setw(8) << food->id;
        cout << " | ";
        cout << left << setw(30) << food->name;
        cout << " | ";
        cout << left << setw(10) << food->price;
        cout << " |" << endl;
    }

    // Overload for backward compatibility with stall_interface.cpp (ignores flag parameter)
    void displayFoodFormat(int count, struct food* food, bool /*unused*/) {
        displayFoodFormat(count, food);
    }

    // Display all food in list
    void displayAllFood() {
        int length = 64;
        struct food* trav = head;
        int count = 0;
        displayFoodHeader(length);
        while (trav != nullptr) {
            count++;
            displayFoodFormat(count, trav);
            trav = trav->next;
        }
        cout << string(length, '=') << endl << endl;
    }

    // Overload for backward compatibility (ignores flag parameter)
    void displayAllFood(bool /*unused*/) {
        displayAllFood();
    }

    // (Implemented in stall_interface.cpp)
    int displayFoodByStall(int stallId, Food_Stall_Map_Linked_List& mapList);
    int displayAvailableExistingFood(int currentStallId, Food_Stall_Map_Linked_List& mapList);


    void saveOrderMapFood(ofstream& out, int orderID) {
        food* current = head;
        while (current != nullptr) {
            out << orderID << "," << current->id << endl;
            current = current->next;
        }
    }
};