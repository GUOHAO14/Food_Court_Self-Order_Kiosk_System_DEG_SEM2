#pragma once
#include <iostream>
#include <iomanip>

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

	food(int id, string name, double price, int quantity) {
		this->id = id;
		this->name = name;
		this->price = price;
		this->quantity = quantity;
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

	// search for a food item by its ID
	struct food* searchFoodById(int id) {
		struct food* trav = head;

		while (trav != nullptr) {
			if (trav->id == id) {
				return trav;
			}
			trav = trav->next;
		}
		return nullptr; // return nullptr if food not found
	}

	// header for food display
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

	// actual display for food data
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

	// display all food in a formatted table
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
};
