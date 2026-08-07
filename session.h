#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include "food.h"
#include "queue.h"
#include "order_session_stack.h"

using namespace std;

class Session {
private:
	int studentId;
	string loginTime;
	Session_Stack history;
	int ordersCompletedThisSession;
	int currentPage;

	string currentTimeString() {
		time_t now = time(nullptr);
		char buffer[26];
		ctime_s(buffer, sizeof(buffer), &now);
		string result = buffer;
		if (!result.empty() && result.back() == '\n') {
			result.pop_back();
		}
		return result;
	}

	int collectCurrentOrderSteps(struct SessionStep* buffer[], int maxSize) {
		int itemCount = 0;
		struct SessionStep* trav = history.peek();

		while (trav != nullptr) {
			if (trav->type == STEP_LOGIN || trav->type == STEP_CHECKOUT) {
				break;
			}
			if (trav->type == STEP_ADD_ITEM && itemCount < maxSize) {
				buffer[itemCount] = trav;
				itemCount++;
			}
			trav = trav->next;
		}
		return itemCount;
	}

public:
	Session(int studentId) {
		this->studentId = studentId;
		this->ordersCompletedThisSession = 0;
		this->currentPage = 1;
		this->loginTime = currentTimeString();

		history.push(STEP_LOGIN, "Logged in as Student TP" + to_string(studentId), 1);
	}

	int getStudentId() { return studentId; }
	Session_Stack* getHistory() { return &history; }
	int getOrdersCompletedThisSession() { return ordersCompletedThisSession; }
	int getCurrentPage() { return currentPage; }
	void setCurrentPage(int pageId) { this->currentPage = pageId; }

	// Session Recording Methods 

	void recordPageNavigation(int targetPage, string pageName) {
		this->currentPage = targetPage;
		history.push(STEP_NAVIGATE, "Navigated to " + pageName, targetPage);
	}

	void recordViewMenu() {
		history.push(STEP_VIEW_MENU, "Browsed food menu", 2);
	}

	void recordViewCart() {
		history.push(STEP_VIEW_CART, "Viewed current order cart", 3);
	}

	void recordViewStalls() {
		history.push(STEP_VIEW_STALLS, "Displayed stall status list", currentPage);
	}

	void recordViewCircularQueue() {
		history.push(STEP_VIEW_CIRCULAR_QUEUE, "Displayed circular queue status", currentPage);
	}

	void recordViewOrderQueues() {
		history.push(STEP_VIEW_ORDER_QUEUES, "Viewed all system order queues", currentPage);
	}

	void recordViewMyStallAssignments() {
		history.push(STEP_VIEW_MY_STALL_ASSIGNMENTS, "Viewed my orders", currentPage);
	}

	void recordViewOrderStallAssignment() {
		history.push(STEP_VIEW_ORDER_STALL_ASSIGNMENT, "Viewed stall food assignment list", currentPage);
	}

	void recordViewHistory() {
		history.push(STEP_VIEW_HISTORY, "Viewed session history log", currentPage);
	}

	void addItemToOrder(struct food* selectedFood) {
		string desc = "Added 1x " + selectedFood->name + " to order";
		history.pushItem(STEP_ADD_ITEM, desc, currentPage, selectedFood->id, selectedFood->name, selectedFood->price);
	}

	bool goBackOneStep(string& undoneDescription, int& targetPage) {
		if (history.isEmpty() || (history.getCount() <= 1 && history.peek()->type == STEP_LOGIN)) {
			return false;
		}

		SessionStep popped(STEP_LOGIN, "", 1);
		if (!history.pop(popped)) {
			return false;
		}

		undoneDescription = popped.description;

		struct SessionStep* topStep = history.peek();
		if (topStep != nullptr) {
			targetPage = topStep->pageId;
		}
		else {
			targetPage = 1;
		}

		this->currentPage = targetPage;
		return true;
	}

	int getCurrentOrderItemCount() {
		struct SessionStep* buffer[200];
		return collectCurrentOrderSteps(buffer, 200);
	}

	double getCurrentOrderTotal() {
		struct SessionStep* buffer[200];
		int itemCount = collectCurrentOrderSteps(buffer, 200);

		double total = 0.0;
		for (int i = 0; i < itemCount; i++) {
			total += buffer[i]->price;
		}
		return total;
	}

	void displayCurrentOrder() {
		struct SessionStep* buffer[200];
		int itemCount = collectCurrentOrderSteps(buffer, 200);

		if (itemCount == 0) {
			cout << "Your order cart is currently empty." << endl;
			return;
		}

		Food_Linked_List formatter;
		int length = 64;
		formatter.displayFoodHeader(true, length);

		int rowNum = 0;
		for (int i = itemCount - 1; i >= 0; i--) {
			rowNum++;
			food rowItem(buffer[i]->foodId, buffer[i]->foodName, buffer[i]->price);
			formatter.displayFoodFormat(rowNum, &rowItem, true);
		}
		cout << string(length, '=') << endl;
		cout << "Order Total: RM" << fixed << setprecision(2) << getCurrentOrderTotal() << endl << endl;
	}

	Order* checkoutCurrentOrder(int orderId) {
		struct SessionStep* buffer[200];
		int itemCount = collectCurrentOrderSteps(buffer, 200);

		if (itemCount == 0) {
			return nullptr;
		}

		double orderTotal = 0.0;
		Order* newOrder = new Order(orderId, studentId, currentTimeString(), "Pending");

		for (int i = itemCount - 1; i >= 0; i--) {
			newOrder->getFoodList()->insertRear(buffer[i]->foodId, buffer[i]->foodName, buffer[i]->price);
			orderTotal += buffer[i]->price;
		}

		ordersCompletedThisSession++;

		ostringstream oss;
		oss << "Checked out order #" << orderId << " (" << itemCount << " item(s), Total: RM"
			<< fixed << setprecision(2) << orderTotal << ")";

		history.push(STEP_CHECKOUT, oss.str(), 4);

		return newOrder;
	}
};