#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

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
		}

		struct food* prev = nullptr, * deleteNode = head;

		while (deleteNode != nullptr) {
			if (deleteNode->id == id) {
				break;
			}
			prev = deleteNode;
			deleteNode = deleteNode->next; // deleteNode will be tail at the end of the loop
		}

		if (deleteNode == nullptr) { // reach end of list without finding the id
			cout << "Food ID \"" << id << "\" is not found." << endl;
		}
		else {
			if (deleteNode == head) {
				head = head->next;

				if (deleteNode == tail) { // if one node
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

	void detachFront() { // used for food waiting queue
		if (head == nullptr) {
			cout << "Cannot delete from an empty list." << endl;
			return;
		}
		struct food* deleteNode = head;
		string deletedId;

		if (head->next == nullptr) { // one node
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

	// actual display for food data
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

	// display all food in a formatted table
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

	void saveOrderMapFood(ofstream& out, int orderID)
	{
		food* current = head;
		while (current != nullptr)
		{
			out << orderID << "," << current->id << endl;
			current = current->next;
		}
	}
};
