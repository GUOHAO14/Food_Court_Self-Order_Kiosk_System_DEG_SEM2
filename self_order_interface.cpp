#include "self_order_interface.h"
#include "globals.h"
#include "stall_assignment.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include "utils.h"
#include "session.h"
#include "order_session_stack.h"


using namespace std;

// Maximum allowed food items in a single order
const int MAX_ORDER_ITEMS = 8;

// Initialized to -1 so we can auto-detect the next ID from CSV on first run
static int nextOrderId = -1;

// Forward Declarations
void page1_MainHub(Session* session);
void page2_FoodMenu(Session* session);
void page3_CartReview(Session* session);
void page4_CheckoutConfirmation(Session* session);

void addItemToOrderFlow(Session* session);
void processFinalCheckout(Session* session);
bool performUndo(Session* session);
int getMaxOrderIdFromCSV(const char* filename);
void displayAllOrders();
int getValidIntInput();

// Raw Stream Input Validation 
int getValidIntInput() {
    int value;
    while (!(cin >> value)) {
        cin.clear();
        while (cin.get() != '\n'); // Flush input stream buffer manually
        cout << "[Invalid Input] Please enter a valid number: ";
    }
    return value;
}

// Display All Queues Directly 
void displayAllOrders() {
    cout << "\n=============================================" << endl;
    cout << "          ORDER QUEUES STATUS          " << endl;
    cout << "=============================================" << endl;

    cout << "\n--- PENDING ORDERS QUEUE (" << pendingOrdersQueue.queueNum() << ") ---" << endl;
    if (pendingOrdersQueue.queueNum() == 0) {
        cout << "  (No pending orders in queue)" << endl;
    }
    else {
        pendingOrdersQueue.displayQueue();
    }

    cout << "\n--- PROCESSING ORDERS QUEUE (" << processingOrdersQueue.queueNum() << ") ---" << endl;
    if (processingOrdersQueue.queueNum() == 0) {
        cout << "  (No processing orders in queue)" << endl;
    }
    else {
        processingOrdersQueue.displayQueue();
    }

    cout << "\n--- COMPLETED ORDERS QUEUE (" << completedOrdersQueue.queueNum() << ") ---" << endl;
    if (completedOrdersQueue.queueNum() == 0) {
        cout << "  (No completed orders in queue)" << endl;
    }
    else {
        completedOrdersQueue.displayQueue();
    }

    cout << "=============================================" << endl;
}

// CSV Order ID Detection
int getMaxOrderIdFromCSV(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return 100;
    }

    char line[256];
    int maxId = 100;

    while (file.getline(line, sizeof(line))) {
        if (line[0] == '\0') continue;

        // Skip header lines 
        if (line[0] == 'o' || line[0] == 'O') continue;

        int currentId = 0;
        int i = 0;

        // Parse first column integer up to comma or line break
        while (line[i] != '\0' && line[i] != ',' && line[i] != '\r' && line[i] != '\n') {
            if (line[i] >= '0' && line[i] <= '9') {
                currentId = currentId * 10 + (line[i] - '0');
            }
            i++;
        }

        if (currentId > maxId) {
            maxId = currentId;
        }
    }

    file.close();
    return maxId;
}

bool performUndo(Session* session) {
    string undoneMsg;
    int targetPage;

    if (session->goBackOneStep(undoneMsg, targetPage)) {
        cout << "\n=============================================" << endl;
        cout << "[UNDO SUCCESSFUL]" << endl;
        cout << "Reversed Action : \"" << undoneMsg << "\"" << endl;
        cout << "Redirecting to   : Page " << targetPage << endl;
        cout << "=============================================" << endl;
        return true;
    }
    else {
        cout << "\n[UNDO NOTICE] Nothing left to undo in this session." << endl;
        return false;
    }
}

// MAIN ENTRY POINT 
void printSelfOrderInt(int stuId) {
    if (nextOrderId == -1) {
        int highestId = getMaxOrderIdFromCSV("order.csv");
        nextOrderId = (highestId >= 100) ? (highestId + 1) : 101;
    }

    Session session(stuId);

    bool activeSession = true;
    while (activeSession) {
        switch (session.getCurrentPage()) {
        case 1:
            page1_MainHub(&session);
            break;
        case 2:
            page2_FoodMenu(&session);
            break;
        case 3:
            page3_CartReview(&session);
            break;
        case 4:
            page4_CheckoutConfirmation(&session);
            break;
        case 0:
            activeSession = false;
            break;
        default:
            session.setCurrentPage(1);
            break;
        }
    }
}

void page1_MainHub(Session* session) {
    cout << "\n=============================================" << endl;
    cout << "        PAGE 1: KIOSK MAIN HUB                " << endl;
    cout << "=============================================" << endl;
    cout << "Logged in as Student: TP" << session->getStudentId() << endl;

    int pendingItems = session->getCurrentOrderItemCount();
    if (pendingItems > 0) {
        cout << "Notice: You have " << pendingItems << "/" << MAX_ORDER_ITEMS << " item(s) currently in your cart." << endl;
    }

    cout << "\n1. Start Ordering" << endl;
    cout << "2. Display Stall Status" << endl;
    cout << "3. Display Circular Queue" << endl;
    cout << "4. View All Order Queues (Pending, Processing, Completed)" << endl;
    cout << "5. View My Orders" << endl;
    cout << "6. View Stall Food Assignment List" << endl;
    cout << "7. View Kiosk Session History Log" << endl;
    cout << "8. Undo Last Action" << endl;
    cout << "9. Exit Session" << endl;
    cout << "Choice: ";

    int choice = getValidIntInput();

    switch (choice) {
    case 1:
        session->recordPageNavigation(2, "Food Menu (Page 2)");
        break;
    case 2:
        session->recordViewStalls();
        stallList.displayAllStalls();
        break;
    case 3:
        session->recordViewCircularQueue();
        stallCircularQueue.displayQueue();
        break;
    case 4:
        session->recordViewOrderQueues();
        displayAllOrders();
        break;
    case 5:
        session->recordViewMyStallAssignments();
        displayStudentOrderStallAssignment(session->getStudentId());
        break;
    case 6:
        session->recordViewOrderStallAssignment();
        stallList.displayAllStallAssignment();
        break;
    case 7:
        session->recordViewHistory();
        session->getHistory()->displayHistory();
        break;
    case 8:
        performUndo(session);
        break;
    case 9:
        cout << "\nExiting kiosk session. Goodbye!" << endl;
        session->setCurrentPage(0);
        break;
    default:
        cout << "Invalid selection. Please try again." << endl;
        break;
    }
}

void page2_FoodMenu(Session* session) {
    cout << "\n=============================================" << endl;
    cout << "        PAGE 2: ORDER FOOD (MENU)              " << endl;
    cout << "=============================================" << endl;

    // REMOVED: session->recordViewMenu(); 
    // Page navigation to Page 2 is already recorded when transitioning.

    foodList.displayAllFood(false);

    int pendingItems = session->getCurrentOrderItemCount();
    cout << "\nCart Status: " << pendingItems << "/" << MAX_ORDER_ITEMS << " item(s) selected." << endl;

    cout << "\n1. Select & Add Food Item" << endl;
    cout << "2. Proceed to Cart Review (Page 3)" << endl;
    cout << "3. Undo Last Action" << endl;
    cout << "4. Back to Main Hub (Page 1)" << endl;
    cout << "Choice: ";

    int choice = getValidIntInput();

    switch (choice) {
    case 1:
        addItemToOrderFlow(session);
        break;
    case 2:
        if (session->getCurrentOrderItemCount() == 0) {
            cout << "\n[Notice] Your cart is empty. Add at least one item before proceeding." << endl;
        }
        else {
            session->recordPageNavigation(3, "Cart Review (Page 3)");
        }
        break;
    case 3:
        performUndo(session);
        break;
    case 4:
        session->recordPageNavigation(1, "Main Hub (Page 1)");
        break;
    default:
        cout << "Invalid selection. Try again." << endl;
        break;
    }
}

void page3_CartReview(Session* session) {
    cout << "\n=============================================" << endl;
    cout << "        PAGE 3: CART REVIEW                    " << endl;
    cout << "=============================================" << endl;

    session->displayCurrentOrder();

    cout << "\n1. Proceed to Final Checkout (Page 4)" << endl;
    cout << "2. Back to Food Menu to Add Items (Page 2)" << endl;
    cout << "3. Undo Last Action" << endl;
    cout << "4. Cancel & Return to Main Hub (Page 1)" << endl;
    cout << "Choice: ";

    int choice = getValidIntInput();

    switch (choice) {
    case 1:
        if (session->getCurrentOrderItemCount() == 0) {
            cout << "\n[Notice] Your cart is empty! Cannot proceed to checkout." << endl;
        }
        else {
            session->recordPageNavigation(4, "Checkout (Page 4)");
        }
        break;
    case 2:
        session->recordPageNavigation(2, "Food Menu (Page 2)");
        break;
    case 3:
        performUndo(session);
        break;
    case 4:
        session->recordPageNavigation(1, "Main Hub (Page 1)");
        break;
    default:
        cout << "Invalid option. Try again." << endl;
        break;
    }
}

void page4_CheckoutConfirmation(Session* session) {
    cout << "\n=============================================" << endl;
    cout << "        PAGE 4: CHECKOUT CONFIRMATION         " << endl;
    cout << "=============================================" << endl;

    cout << "Review your final order summary below:\n" << endl;
    session->displayCurrentOrder();

    cout << "\n1. Confirm & Place Order" << endl;
    cout << "2. Undo Last Action" << endl;
    cout << "3. Back to Cart Review (Page 3)" << endl;
    cout << "Choice: ";

    int choice = getValidIntInput();

    switch (choice) {
    case 1:
        processFinalCheckout(session);
        session->recordPageNavigation(1, "Main Hub (Page 1)");
        break;
    case 2:
        performUndo(session);
        break;
    case 3:
        session->recordPageNavigation(3, "Cart Review (Page 3)");
        break;
    default:
        cout << "Invalid option. Try again." << endl;
        break;
    }
}

void addItemToOrderFlow(Session* session) {
    int currentCount = session->getCurrentOrderItemCount();

    if (currentCount >= MAX_ORDER_ITEMS) {
        cout << "\n[Order Limit Reached] Your cart already contains the maximum limit of "
            << MAX_ORDER_ITEMS << " items per order." << endl;
        cout << "Please complete checkout for this order before starting a new one." << endl;
        return;
    }

    cout << "\nEnter Food ID to add (0 to cancel): ";
    int foodId = getValidIntInput();

    if (foodId == 0) {
        cout << "Action cancelled." << endl;
        return;
    }

    struct food* selectedFood = foodList.searchFoodById(foodId);
    if (selectedFood == nullptr) {
        cout << "Food ID " << foodId << " not found on menu." << endl;
        return;
    }

    cout << "Enter quantity for \"" << selectedFood->name << "\" (RM"
        << fixed << setprecision(2) << selectedFood->price << " each): ";
    int quantity = getValidIntInput();

    if (quantity <= 0) {
        cout << "Quantity must be at least 1." << endl;
        return;
    }

    if (currentCount + quantity > MAX_ORDER_ITEMS) {
        int spaceLeft = MAX_ORDER_ITEMS - currentCount;
        cout << "\n[Order Limit Exceeded] Cannot add " << quantity << " item(s)." << endl;
        cout << "Your cart currently has " << currentCount << " item(s)." << endl;
        cout << "Maximum limit is " << MAX_ORDER_ITEMS << " items per order. You can only add up to "
            << spaceLeft << " more item(s) to this order." << endl;
        return;
    }

    for (int i = 0; i < quantity; i++) {
        food* newFood = new food(
            selectedFood->id,
            selectedFood->name,
            selectedFood->price
        );

        session->addItemToOrder(newFood);
    }

    cout << "\n[Added] " << quantity << "x \"" << selectedFood->name
        << "\" added to your order (" << (currentCount + quantity) << "/" << MAX_ORDER_ITEMS << " items)." << endl;
}

void processFinalCheckout(Session* session) {
    Order* placedOrder = session->checkoutCurrentOrder(nextOrderId);

    if (placedOrder == nullptr) {
        cout << "\nCheckout failed: Cart was empty." << endl;
        return;
    }

    nextOrderId++;

    cout << "\n=============================================" << endl;
    cout << "          ORDER CONFIRMED & PLACED            " << endl;
    cout << "=============================================" << endl;
    placedOrder->displayOrder();

    pendingOrdersQueue.addQueue(placedOrder);

    stallAndOrderAssignment();

    cout << "\nYour order has been placed into the system queue!" << endl;
    cout << "Total pending orders in queue: " << pendingOrdersQueue.queueNum() << endl;
}