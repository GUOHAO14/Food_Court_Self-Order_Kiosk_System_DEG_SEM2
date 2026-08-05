#include <iostream>
#include <iomanip>
#include <ctime>
#include "order_interface.h"

using namespace std;

int orderCounter = 0;

int stepCounter = 0;

int buildOrderId() {
	orderCounter++;
	return orderCounter;
}

// Zero-padded display form of an order ID, e.g. 7 -> "O007".
string formatOrderId(int id) {
	string numStr = to_string(id);
	while (numStr.length() < 3) {
		numStr = "0" + numStr;
	}
	return "O" + numStr;
}

// Returns the current local time formatted like "Tue Aug  4 11:37:25 2026",
// matching the order_time column already used in the order CSVs.
string currentTimeString() {
	time_t now = time(nullptr);
	char buffer[26];

#if defined(_WIN32)
	ctime_s(buffer, sizeof(buffer), &now);
	string result = buffer;
#else
	ctime_r(&now, buffer);
	string result = buffer;
#endif

	// ctime_s / ctime_r both add a trailing '\n'; trim it.
	if (!result.empty() && result.back() == '\n') {
		result.pop_back();
	}
	return result;
}

//for login/browse menu steps
bool logStep(Stack* sessionHistoryPtr, string studentId, string action) {
	string stepId = "S" + string(3 - to_string(stepCounter + 1).length(), '0') + to_string(stepCounter + 1);
	bool pushed = sessionHistoryPtr->pushStep(SessionStep(stepId, studentId, action));
	if (pushed) {
		stepCounter++;
		cout << "  [Session] " << stepId << " pushed -> " << action
			<< "  (steps so far: " << sessionHistoryPtr->stackSize() << "/" << Stack::MAX_HISTORY_SIZE << ")" << endl;
	}
	return pushed;
}

//for order item steps
bool logItemStep(Stack* sessionHistoryPtr, string studentId, string action, int foodId, string foodName, double price, int quantity) {
	string stepId = "S" + string(3 - to_string(stepCounter + 1).length(), '0') + to_string(stepCounter + 1);
	bool pushed = sessionHistoryPtr->pushStep(SessionStep(stepId, studentId, action, foodId, foodName, price, quantity));
	if (pushed) {
		stepCounter++;
		cout << "  [Session] " << stepId << " pushed -> " << action
			<< "  (steps so far: " << sessionHistoryPtr->stackSize() << "/" << Stack::MAX_HISTORY_SIZE << ")" << endl;
	}
	return pushed;
}

SessionStep undoStep(Stack* sessionHistoryPtr) {
	SessionStep undone = sessionHistoryPtr->popStep();
	cout << "  [Session] " << undone.getStepID() << " popped -> undid \"" << undone.getAction() << "\""
		<< "  (steps left: " << sessionHistoryPtr->stackSize() << ")" << endl;
	return undone;
}

void printSessionHistory(Stack* sessionHistoryPtr, string studentId) {
	cout << endl << "=============== Session History (Student " << studentId << ") ===============" << endl;
	sessionHistoryPtr->displayHistory(studentId);
	cout << "Total Steps Recorded For This Student: " << sessionHistoryPtr->stackSizeForStudent(studentId) << endl;
}

string buildCartString(Stack* sessionHistoryPtr, string studentId) {
	return sessionHistoryPtr->peekCartSummaryForStudent(studentId);
}

Order* orderInterface(Queue* pendingQueuePtr, Food_Linked_List* foodListPtr, Stack* sessionHistoryPtr) {
	int studentId;
	cout << endl << "=============== Self-Order Kiosk ===============" << endl;
	cout << "Please enter your Student ID (numeric): ";
	cin >> studentId;
	string studentIdStr = to_string(studentId);

	int itemCount = sessionHistoryPtr->countActiveCartItemsForStudent(studentIdStr);

	if (itemCount > 0) {
		cout << endl << "Welcome back, Student " << studentIdStr << "! Restoring your saved cart: "
			<< sessionHistoryPtr->peekCartSummaryForStudent(studentIdStr) << endl;
	}
	else {
		logStep(sessionHistoryPtr, studentIdStr, "Logged in / Scanned Student ID");
		logStep(sessionHistoryPtr, studentIdStr, "Browsed Menu");
	}

	bool ordering = true;

	while (ordering) {
		cout << endl << "-------------------------------------------------" << endl;
		cout << "Menu:" << endl;
		foodListPtr->displayAllFood(true);

		if (itemCount > 0) {
			cout << "Current Cart: " << buildCartString(sessionHistoryPtr, studentIdStr) << endl;
		}
		cout << "Session history: " << sessionHistoryPtr->stackSizeForStudent(studentIdStr) << "/" << Stack::MAX_HISTORY_SIZE << " steps used (your steps)" << endl;
		cout << "Enter Food ID to add (0 = finish, -1 = undo last item, -2 = view full session history, -3 = edit quantity of last item, -4 = save cart & return to main menu): ";

		int foodId;
		if (!(cin >> foodId)) {

			cin.clear();
			cin.ignore(10000, '\n');
			cout << "Invalid input. Please enter a numeric Food ID." << endl;
			continue;
		}

		if (foodId == 0) {
			ordering = false;
			break;
		}

		if (foodId == -1) {

			if (itemCount == 0) {
				cout << "Nothing to undo - your cart is already empty." << endl;
			}
			else {
				SessionStep undone = undoStep(sessionHistoryPtr);
				itemCount--;
				cout << "Navigated back. Removed from cart: " << undone.getFoodName() << " x" << undone.getQuantity() << endl;
			}
			continue;
		}

		if (foodId == -2) {

			printSessionHistory(sessionHistoryPtr, studentIdStr);
			continue;
		}

		if (foodId == -4) {

			if (itemCount == 0) {
				cout << "Your cart is empty - nothing to save. Returning to main menu." << endl;
			}
			else {
				cout << "Cart saved: " << sessionHistoryPtr->peekCartSummaryForStudent(studentIdStr) << endl;
				cout << "Enter the same Student ID (" << studentIdStr << ") next time to pick up where you left off." << endl;
			}
			return nullptr;
		}

		if (foodId == -3) {

			if (itemCount == 0) {
				cout << "Nothing to edit - your cart is currently empty." << endl;
				continue;
			}

			SessionStep lastStep = sessionHistoryPtr->peekStep();
			if (!lastStep.isItemStep()) {
				cout << "The last recorded step is not an item selection, so there is nothing to edit." << endl;
				continue;
			}

			cout << "Editing last selected item: \"" << lastStep.getFoodName()
				<< "\" (current quantity: " << lastStep.getQuantity() << ")" << endl;
			cout << "Enter new quantity (0 = cancel edit): ";

			int newQuantity;
			if (!(cin >> newQuantity)) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Invalid input. Edit cancelled." << endl;
				continue;
			}

			if (newQuantity <= 0) {
				cout << "Edit cancelled. Quantity unchanged." << endl;
				continue;
			}

			sessionHistoryPtr->popStep();

			bool updated = logItemStep(sessionHistoryPtr, studentIdStr,
				"Updated Quantity: " + lastStep.getFoodName() + " x" + to_string(newQuantity),
				lastStep.getFoodId(), lastStep.getFoodName(), lastStep.getPrice(), newQuantity);

			if (updated) {
				cout << "Quantity updated. \"" << lastStep.getFoodName() << "\" is now x" << newQuantity << " in your cart." << endl;
			}
			else {
				// Extremely unlikely (popping just freed a slot), but restore
				// the original entry so the item is not silently lost.
				logItemStep(sessionHistoryPtr, studentIdStr, lastStep.getAction(),
					lastStep.getFoodId(), lastStep.getFoodName(), lastStep.getPrice(), lastStep.getQuantity());
				cout << "Could not record the edit because the session history limit was reached. Quantity left unchanged." << endl;
			}
			continue;
		}

		struct food* selectedFood = foodListPtr->searchFoodById(foodId);
		if (selectedFood == nullptr) {
			cout << "Food ID " << foodId << " was not found on the menu. Please try again." << endl;
			continue;
		}

		int quantity;
		cout << "Enter quantity for \"" << selectedFood->name << "\": ";
		cin >> quantity;

		if (quantity <= 0) {
			cout << "Quantity must be at least 1. Item not added." << endl;
			continue;
		}

		bool added = logItemStep(sessionHistoryPtr, studentIdStr, "Selected Item: " + selectedFood->name + " x" + to_string(quantity),
			selectedFood->id, selectedFood->name, selectedFood->price, quantity);

		if (added) {
			itemCount++;
		}
		else {
			cout << "Item not added. Please finish your order, undo (-1), or edit (-3) an existing item to free up space." << endl;
		}
	}

	if (itemCount == 0) {
		cout << endl << "No items were selected. Order cancelled." << endl;
		logStep(sessionHistoryPtr, studentIdStr, "Cancelled Order (No Items Selected)");
		return nullptr;
	}

	string cart = buildCartString(sessionHistoryPtr, studentIdStr);
	int orderId = buildOrderId();
	string orderTime = currentTimeString();

	Order newOrder(orderId, studentId, orderTime);

	// Rebuild the food list for this order from the student's still-open
	// cart steps in the session history (this correctly skips over any
	// other students' steps that may have been interleaved in between, e.g.
	// if this student saved & exited earlier). Food_Linked_List::insertRear
	// does not keep a per-line quantity, so each unit is added as its own
	// row - this matches the "one row per food_id" style already used in
	// pending_order_map_food.csv / completed_order_map_food.csv.
	double orderTotal = 0.0;
	SessionStep cartItems[Stack::MAX_HISTORY_SIZE];
	int cartItemCount = sessionHistoryPtr->getActiveCartItemsForStudent(studentIdStr, cartItems);
	for (int i = 0; i < cartItemCount; i++) {
		SessionStep& item = cartItems[i];
		for (int q = 0; q < item.getQuantity(); q++) {
			newOrder.addFood(food(item.getFoodId(), item.getFoodName(), item.getPrice()));
		}
		orderTotal += item.getPrice() * item.getQuantity();
	}

	pendingQueuePtr->addQueue(newOrder);
	Order* placedOrder = pendingQueuePtr->searchOrderById(orderId);

	logStep(sessionHistoryPtr, studentIdStr, "Order Placed (Order ID: " + formatOrderId(orderId) + ")");

	cout << endl << "=============== Order Confirmed ===============" << endl;
	cout << "Order ID : " << formatOrderId(orderId) << endl;
	cout << "Student  : " << studentId << endl;
	cout << "Time     : " << orderTime << endl;
	cout << "Items    : " << cart << endl;
	cout << "Total    : RM" << fixed << setprecision(2) << orderTotal << endl;
	cout << "Your order has been placed and is now in the pending queue." << endl;
	cout << "Pending orders in queue: " << pendingQueuePtr->queueNum() << endl;

	return placedOrder;
}
