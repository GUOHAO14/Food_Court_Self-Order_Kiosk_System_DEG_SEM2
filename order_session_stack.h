#pragma once
#include <iostream>
#include <string>
using namespace std;

// Enumeration of all kiosk session actions
enum StepType {
	STEP_LOGIN,                 // Student logged into kiosk (Page 1 base)
	STEP_NAVIGATE,              // Navigated between pages
	STEP_VIEW_MENU,             // Opened food menu (Page 2)
	STEP_ADD_ITEM,              // Added single food item to order
	STEP_VIEW_CART,             // Viewed current order/cart (Page 3)
	STEP_VIEW_STALLS,           // Viewed stall status list
	STEP_VIEW_CIRCULAR_QUEUE,   // Viewed circular queue status
	STEP_VIEW_ORDER_QUEUES,     // Viewed all system order queues
	STEP_VIEW_HISTORY,          // Viewed kiosk session history log
	STEP_CHECKOUT               // Completed order checkout (Page 4)
};

// Represents a single action step on the history stack
struct SessionStep {
	StepType type;
	string description;
	int pageId; // 1: Main Hub, 2: Menu, 3: Cart, 4: Checkout

	int foodId;
	string foodName;
	double price;

	struct SessionStep* next;

	// Plain step constructor
	SessionStep(StepType type, string description, int pageId) {
		this->type = type;
		this->description = description;
		this->pageId = pageId;
		this->foodId = -1;
		this->foodName = "";
		this->price = 0.0;
		this->next = nullptr;
	}

	// Item step constructor
	SessionStep(StepType type, string description, int pageId, int foodId, string foodName, double price) {
		this->type = type;
		this->description = description;
		this->pageId = pageId;
		this->foodId = foodId;
		this->foodName = foodName;
		this->price = price;
		this->next = nullptr;
	}
};

// LIFO Stack that records all student actions and page history
class Session_Stack {
private:
	struct SessionStep* top;
	int count;

public:
	Session_Stack() {
		top = nullptr;
		count = 0;
	}

	bool isEmpty() {
		return top == nullptr;
	}

	int getCount() {
		return count;
	}

	void push(StepType type, string description, int pageId) {
		struct SessionStep* newStep = new SessionStep(type, description, pageId);
		newStep->next = top;
		top = newStep;
		count++;
	}

	void pushItem(StepType type, string description, int pageId, int foodId, string foodName, double price) {
		struct SessionStep* newStep = new SessionStep(type, description, pageId, foodId, foodName, price);
		newStep->next = top;
		top = newStep;
		count++;
	}

	struct SessionStep* peek() {
		return top;
	}

	bool pop(SessionStep& poppedStep) {
		if (isEmpty()) {
			return false;
		}

		struct SessionStep* temp = top;
		poppedStep = *temp;
		top = top->next;
		delete temp;
		count--;
		return true;
	}

	void displayHistory() {
		if (isEmpty()) {
			cout << "No session history recorded yet." << endl;
			return;
		}

		int length = 65;
		cout << string(length, '=') << endl;
		cout << "SESSION HISTORY LOG (Most recent action first)" << endl;
		cout << string(length, '=') << endl;

		struct SessionStep* trav = top;
		int stepNo = count;
		while (trav != nullptr) {
			cout << "[Step " << stepNo << "] [Page " << trav->pageId << "] " << trav->description << endl;
			trav = trav->next;
			stepNo--;
		}
		cout << string(length, '=') << endl << endl;
	}

	~Session_Stack() {
		SessionStep dummy(STEP_LOGIN, "", 1);
		while (pop(dummy)) {}
	}
};