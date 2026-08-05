#include <iostream>
#include <iomanip>
#include <string>
#include "globals.h"
#include "stall_interface.h"

using namespace std;

// Global stall selection pointer
struct stall* selectedStall = nullptr;

// =========================================================
// DEFINITIONS FOR Food_Linked_List MEMBER FUNCTIONS
// =========================================================

// Displays ONLY food items that belong to the specified stall ID
int Food_Linked_List::displayFoodByStall(int stallId, Food_Stall_Map_Linked_List& mapList) {
    int length = 64;
    struct food* trav = head;
    int displayCount = 0;

    displayFoodHeader(false, length);

    while (trav != nullptr) {
        if (mapList.checkFoodStallMapping(trav->id, stallId)) {
            displayCount++;
            displayFoodFormat(displayCount, trav, false);
        }
        trav = trav->next;
    }

    cout << string(length, '=') << endl << endl;
    return displayCount;
}

// Displays ONLY food items that are NOT currently sold by the specified stall ID
int Food_Linked_List::displayAvailableExistingFood(int currentStallId, Food_Stall_Map_Linked_List& mapList) {
    int length = 64;
    struct food* trav = head;
    int count = 0;

    displayFoodHeader(false, length);

    while (trav != nullptr) {
        if (!mapList.checkFoodStallMapping(trav->id, currentStallId)) {
            count++;
            displayFoodFormat(count, trav, false);
        }
        trav = trav->next;
    }

    cout << string(length, '=') << endl << endl;
    return count;
}

// =========================================================
// STALL MANAGEMENT USER INTERFACE FUNCTIONS
// =========================================================

void chooseStall() {
    int choice;

    do {
        cout << endl << "=============== Choose Stall ===============" << endl;
        cout << "1. Western" << endl;
        cout << "2. Asian" << endl;
        cout << "3. Chinese" << endl;
        cout << "4. Mamak" << endl;
        cout << "5. Indian" << endl;
        cout << "6. Back " << endl;

        cout << "Enter your choice (type integer): ";
        cin >> choice;

        if (choice < 1 || choice > 6) {
            cout << "Invalid input. Please try again." << endl;
        }
        else if (choice == 6) {
            // Go back
        }
        else {
            selectedStall = stallList.searchStallById(choice);
            if (selectedStall != nullptr) {
                printManageStallInt();
            }
            else {
                cout << "Stall not found." << endl;
            }
        }
    } while (choice != 6);
}

void printManageStallInt() {
    int choice;
    do {
        cout << endl << "=============== Stall Management ===============" << endl;
        cout << "Hello, you are stall " << selectedStall->name << " (ID: " << selectedStall->id << ")." << endl;
        cout << "1. Set Stall Status" << endl;
        cout << "2. Manage Order Status" << endl;
        cout << "3. Add Menu Item" << endl;
        cout << "4. Remove Menu Item" << endl;
        cout << "5. Update Menu Item" << endl;
        cout << "6. Search Menu Item" << endl;
        cout << "7. Back" << endl;

        cout << "Enter your choice (type integer): ";
        cin >> choice;

        switch (choice) {
        case 1:
            printSetStallStatusInt();
            break;
        case 2:
            break;
        case 3:
            addMenuItem();
            break;
        case 4:
            removeMenuItem();
            break;
        case 5:
            updateMenuItem();
            break;
        case 6:
            searchMenuItem();
            break;
        case 7:
            break;
        default:
            cout << "Invalid input. Please try again." << endl;
        }

    } while (choice != 7);
}

void printSetStallStatusInt() {
    int choice;
    do {
        cout << endl << "=============== Set Stall Status ===============" << endl;
        cout << "1. Open Stall" << endl;
        cout << "2. Close Stall" << endl;
        cout << "3. Back" << endl;
        cout << "Enter your choice (1 for Open, 2 for Close, 3 for Back): ";
        cin >> choice;

        switch (choice) {
        case 1:
            stallList.setStallStatus(selectedStall, true);
            break;
        case 2:
            stallList.setStallStatus(selectedStall, false);
            break;
        case 3:
            break;
        default:
            cout << "Invalid input. Please try again." << endl;
        }

    } while (choice != 3);
}

void addMenuItem() {
    if (selectedStall == nullptr) {
        cout << "No stall selected." << endl;
        return;
    }

    int subChoice;
    cout << "\n================ Add Menu Item ================" << endl;
    cout << "1. Add a Completely New Menu Item" << endl;
    cout << "2. Add an Existing Item Sold by Other Stalls" << endl;
    cout << "3. Cancel" << endl;
    cout << "Enter choice: ";

    while (!(cin >> subChoice) || subChoice < 1 || subChoice > 3) {
        cout << "Invalid choice! Enter 1, 2, or 3: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    if (subChoice == 3) {
        cout << "Operation cancelled." << endl;
        return;
    }

    // Clear leftover newline before reading strings
    if (cin.peek() == '\n') {
        cin.ignore();
    }

    // =========================================================
    // OPTION 1: ADD COMPLETELY NEW MENU ITEM
    // =========================================================
    if (subChoice == 1) {
        string foodName;
        double foodPrice;

        cout << "\n--- Add New Menu Item for " << selectedStall->name << " ---" << endl;

        // Loop for name input and duplicate checking
        while (true) {
            cout << "Enter Food Name (or type 'c' to exit): ";
            getline(cin, foodName);

            if (foodName == "c" || foodName == "C") {
                cout << "Operation cancelled." << endl;
                return;
            }

            if (foodName.empty()) {
                cout << "Food name cannot be empty. Try again.\n";
                continue;
            }

            // Duplicate name check
            if (foodList.isFoodNameExists(foodName)) {
                cout << "An item with the name '" << foodName
                    << "' already exists in the system!\n"
                    << "If another stall sells this, please use Option 2 instead.\n\n";
            }
            else {
                break; // Name is unique
            }
        }

        cout << "Enter Price: RM (or type -1 to cancel): ";

        while (true) {
            if (cin >> foodPrice) {
                if (foodPrice == -1) {
                    cout << "Operation cancelled." << endl;
                    return;
                }
                if (foodPrice >= 0) {
                    break; // Valid price entered
                }
            }

            cout << "Invalid price! Enter a valid non-negative number: RM (or -1 to cancel): ";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        // Generate new food ID
        int newFoodId = (foodList.getTail() != nullptr) ? (foodList.getTail()->id + 1) : 1;

        // Add to main food list
        foodList.insertRear(newFoodId, foodName, foodPrice);

        // Add mapping entry
        foodStallMapList.insertRear(newFoodId, selectedStall->id);

        cout << "New item '" << foodName << "' successfully added to stall " << selectedStall->name << "!" << endl;
    }

    // =========================================================
    // OPTION 2: ADD EXISTING ITEM FROM OTHER STALLS
    // =========================================================
    else if (subChoice == 2) {
        cout << "\n--- Available Existing Items for " << selectedStall->name << " ---" << endl;

        int availableCount = foodList.displayAvailableExistingFood(selectedStall->id, foodStallMapList);

        if (availableCount == 0) {
            cout << "No existing items available to add (your stall already sells all existing menu items)." << endl;
            return;
        }

        int targetFoodId;
        cout << "Enter Food ID to add to your menu (or type -1 to cancel): ";

        while (true) {
            if (!(cin >> targetFoodId)) {
                cout << "Invalid input! Please enter a numeric Food ID: ";
                cin.clear();
                cin.ignore(10000, '\n');
            }
            else if (targetFoodId == -1) {
                cout << "Operation cancelled." << endl;
                return;
            }
            // Check 1: Does this food item even exist in global food list?
            else if (foodList.searchFoodById(targetFoodId) == nullptr) {
                cout << "Food ID not found in the global menu! Try again (or -1 to cancel): ";
            }
            // Check 2: Does this stall already sell it?
            else if (foodStallMapList.checkFoodStallMapping(targetFoodId, selectedStall->id)) {
                cout << "Your stall already sells this item! Pick an item from the table above (or -1 to cancel): ";
            }
            else {
                break; // Valid selection
            }
        }

        // Only add mapping entry (no new food node created)
        foodStallMapList.insertRear(targetFoodId, selectedStall->id);

        struct food* addedFood = foodList.searchFoodById(targetFoodId);
        cout << "Existing item '" << addedFood->name << "' successfully added to stall " << selectedStall->name << "!" << endl;
    }
}

void removeMenuItem() {
    if (selectedStall == nullptr) {
        cout << "No stall selected." << endl;
        return;
    }

    cout << "\n================ Menu for " << selectedStall->name << " ================" << endl;

    int countDisplayed = foodList.displayFoodByStall(selectedStall->id, foodStallMapList);

    if (countDisplayed == 0) {
        cout << "This stall currently has no menu items." << endl;
        return;
    }

    int targetId;
    cout << "Enter the Food ID you want to remove (or type -1 to cancel): ";

    while (true) {
        if (!(cin >> targetId)) {
            cout << "Invalid input! Please enter a numeric Food ID: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        else if (targetId == -1) {
            cout << "Operation cancelled." << endl;
            return;
        }
        else if (!foodStallMapList.checkFoodStallMapping(targetId, selectedStall->id)) {
            cout << "Food ID not found under " << selectedStall->name
                << "! Please enter a valid ID from the list above (or -1 to cancel): ";
        }
        else {
            break;
        }
    }

    // Step 1: Always delete the relationship mapping from this specific stall
    foodStallMapList.deleteMap(targetId, selectedStall->id);

    // Step 2: Check if ANY other stall still sells this food item
    bool usedByOtherStalls = foodStallMapList.isFoodUsedByAnyStall(targetId);

    // Step 3: Delete from main foodList ONLY if no other stall sells it
    if (!usedByOtherStalls) {
        foodList.deleteFoodById(targetId);
        cout << "Food item deleted entirely from the system." << endl;
    }
    else {
        cout << "Food item removed from " << selectedStall->name << " (other stalls still serve this item)." << endl;
    }
}

void updateMenuItem() {
    // TODO: Implementation for updating price or details
}

void searchMenuItem() {
    // TODO: Implementation for searching items within the stall
}