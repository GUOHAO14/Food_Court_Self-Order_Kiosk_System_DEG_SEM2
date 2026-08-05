#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include "globals.h"
#include "admin.h"
#include "stall.h"
#include "food.h"
#include "food_stall_map.h"

using namespace std;

// Global stall selection pointer
extern struct stall* selectedStall;

void adminPage() {
    int choice;
    do {
        cout << endl << "=============== Select ===============" << endl;
        cout << "1. Add Menu Item" << endl;
        cout << "2. Remove Menu Item" << endl;
        cout << "3. Update Menu Item" << endl;
        cout << "4. Search Menu Item" << endl;
        cout << "5. Back" << endl;

        cout << "Enter your choice (type integer): ";
        cin >> choice;

        switch (choice) {
        case 1:
            addMenuItem();
            break;
        case 2:
            removeMenuItem();
            break;
        case 3:
            updateMenuItem();
            break;
        case 4:
            searchMenuItem();
            break;
        case 5:
            break;
        default:
            cout << "Invalid input. Please try again." << endl;
        }

    } while (choice != 5);
}

void addMenuItem() {
    string name;
    double price;

    cout << "\n================ Add Menu Item ================" << endl;
    cout << "(Note: Type 'cancel' or '0' at any prompt to cancel)\n" << endl;

    // Clear newline buffer before reading full line
    cin.ignore(10000, '\n');

    // ----------------------------------------------------
    // 1. Prompt for Food Name & Check for duplicates / cancel
    // ----------------------------------------------------
    while (true) {
        cout << "Enter food name: ";
        getline(cin, name);

        if (name == "cancel" || name == "0") {
            cout << "Operation cancelled. Returning to admin menu..." << endl;
            return;
        }

        if (name.empty()) {
            cout << "Food name cannot be empty. Please enter a valid name." << endl;
            continue;
        }

        if (foodList.isFoodNameExists(name)) {
            cout << "Error: A menu item with the name \"" << name << "\" already exists. Please enter a different name.\n" << endl;
        }
        else {
            break;
        }
    }

    // ----------------------------------------------------
    // 2. Prompt for Food Price or cancel
    // ----------------------------------------------------
    cout << "Enter food price (RM) [or type 0 to cancel]: ";
    while (!(cin >> price) || price < 0) {
        cout << "Invalid price. Please enter a positive number (or 0 to cancel): ";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    if (price == 0) {
        cout << "Operation cancelled. Returning to admin menu..." << endl;
        return;
    }

    // ----------------------------------------------------
    // 3. Generate New Food ID (Find maximum existing ID + 1)
    // ----------------------------------------------------
    int maxId = 0;
    struct food* currentFood = foodList.getHead();
    while (currentFood != nullptr) {
        if (currentFood->id > maxId) {
            maxId = currentFood->id;
        }
        currentFood = currentFood->next;
    }
    int newFoodId = maxId + 1;

    // ----------------------------------------------------
    // 4. Display Available Stalls
    // ----------------------------------------------------
    cout << "\n---------------- Available Stalls ----------------" << endl;
    cout << left << setw(10) << "Stall ID" << " | " << "Stall Name" << endl;
    cout << "--------------------------------------------------" << endl;

    struct stall* currentStall = stallList.getHead();
    if (currentStall == nullptr) {
        cout << "No stalls available to assign!" << endl;
        return;
    }

    while (currentStall != nullptr) {
        cout << left << setw(10) << currentStall->id << " | " << currentStall->name << endl;
        currentStall = currentStall->next;
    }
    cout << "--------------------------------------------------" << endl;

    // ----------------------------------------------------
    // 5. Assign Stalls using foodStallMapList
    // ----------------------------------------------------
    int assignedCount = 0;
    char addMore = 'y';

    while (tolower(addMore) == 'y') {
        int stallId;
        cout << "Enter Stall ID to assign (or 0 to skip/cancel entry): ";
        if (!(cin >> stallId)) {
            cout << "Invalid input. Please enter a numerical Stall ID." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        // Handle typing 0 when asked for Stall ID
        if (stallId == 0) {
            cout << "Stall entry skipped." << endl;
        }
        // Verify stall exists
        else if (stallList.searchStallById(stallId) == nullptr) {
            cout << "Error: Stall ID " << stallId << " does not exist. Please try again." << endl;
            continue;
        }
        // Check if already assigned
        else if (foodStallMapList.checkFoodStallMapping(newFoodId, stallId)) {
            cout << "This food item is already assigned to Stall ID " << stallId << "." << endl;
        }
        else {
            foodStallMapList.insertRear(newFoodId, stallId);
            assignedCount++;
            cout << "Assigned to Stall ID " << stallId << " successfully." << endl;
        }

        // Ask if user wants to add another stall or cancel the entire creation
        cout << "Assign to another stall? (y/n, or c to cancel menu creation): ";
        cin >> addMore;

        if (tolower(addMore) == 'c') {
            cout << "Menu creation cancelled." << endl;
            if (assignedCount > 0) {
                foodStallMapList.deleteByFoodId(newFoodId);
                assignedCount = 0;
            }
            break;
        }
    }

    if (assignedCount == 0) {
        cout << "No stalls assigned. Menu item creation aborted." << endl;
        return;
    }

    // ----------------------------------------------------
    // 6. Update Memory Linked List (foodList)
    // ----------------------------------------------------
    foodList.insertRear(newFoodId, name, price);

    // ----------------------------------------------------
    // 7. Append New Records to CSV Files
    // ----------------------------------------------------
    ofstream foodFile("food.csv", ios::app);
    if (foodFile.is_open()) {
        foodFile << newFoodId << "," << name << "," << price << "\n";
        foodFile.close();
    }
    else {
        cout << "Warning: Unable to open food.csv to persist data." << endl;
    }

    ofstream mapFile("food_stall_map.csv", ios::app);
    if (mapFile.is_open()) {
        struct food_stall_map* currentMap = foodStallMapList.getHead();
        while (currentMap != nullptr) {
            if (currentMap->food_id == newFoodId) {
                mapFile << currentMap->food_id << "," << currentMap->stall_id << "\n";
            }
            currentMap = currentMap->next;
        }
        mapFile.close();
    }
    else {
        cout << "Warning: Unable to open food_stall_map.csv to persist data." << endl;
    }

    cout << "\n[SUCCESS] Menu item \"" << name << "\" (ID: " << newFoodId
        << ") added and assigned to " << assignedCount << " stall(s)!" << endl;
}

void removeMenuItem() {
    struct food* head = foodList.getHead();

    if (head == nullptr) {
        cout << "\nThere are currently no menu items available to remove." << endl;
        return;
    }

    cout << "\n================ Remove Menu Item ================" << endl;

    // 1. Display all food items along with their assigned stalls using food.h & foodStallMapList
    int length = 75;
    cout << string(length, '=') << endl;
    cout << "| " << left << setw(8) << "Food ID"
        << " | " << left << setw(28) << "Food Name"
        << " | " << left << setw(10) << "Price (RM)"
        << " | " << left << setw(18) << "Assigned Stall ID(s)" << " |" << endl;
    cout << string(length, '=') << endl;

    struct food* currentFood = head;
    while (currentFood != nullptr) {
        // Collect assigned stall IDs for the current food item
        string stallIdsStr = "";
        struct food_stall_map* currentMap = foodStallMapList.getHead();
        while (currentMap != nullptr) {
            if (currentMap->food_id == currentFood->id) {
                if (!stallIdsStr.empty()) {
                    stallIdsStr += ", ";
                }
                stallIdsStr += to_string(currentMap->stall_id);
            }
            currentMap = currentMap->next;
        }

        if (stallIdsStr.empty()) {
            stallIdsStr = "None";
        }

        cout << "| " << left << setw(8) << currentFood->id
            << " | " << left << setw(28) << currentFood->name
            << " | " << left << setw(10) << fixed << setprecision(2) << currentFood->price
            << " | " << left << setw(20) << stallIdsStr << " |" << endl;

        currentFood = currentFood->next;
    }
    cout << string(length, '=') << endl;

    // 2. Prompt user to select a Food ID to delete
    int targetId;
    cout << "\nEnter Food ID to delete (or type 0 to cancel): ";
    if (!(cin >> targetId) || targetId <= 0) {
        if (targetId == 0) {
            cout << "Operation cancelled." << endl;
        }
        else {
            cout << "Invalid input." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
        }
        return;
    }

    // 3. Verify existence using searchFoodById from food.h
    struct food* targetFood = foodList.searchFoodById(targetId);
    if (targetFood == nullptr) {
        cout << "Error: Food ID " << targetId << " does not exist." << endl;
        return;
    }

    string deletedName = targetFood->name;

    // 4. Confirmation step
    char confirm;
    cout << "Are you sure you want to delete \"" << deletedName << "\" (ID: " << targetId << ")? (y/n): ";
    cin >> confirm;
    if (tolower(confirm) != 'y') {
        cout << "Deletion cancelled." << endl;
        return;
    }

    // 5. Remove from memory (foodList and foodStallMapList)
    // 5a. Remove from foodList using deleteFoodById defined in food.h
    foodList.deleteFoodById(targetId);

    // 5b. Remove from foodStallMapList (all occurrences of targetId)
    foodStallMapList.deleteByFoodId(targetId);

    // 6. Rewrite CSV files with updated linked lists
    // 6a. Rewrite food.csv
    ofstream foodFile("food.csv", ios::out | ios::trunc);
    if (foodFile.is_open()) {
        foodFile << "food_id,food_name,price\n"; // Header row
        struct food* fNode = foodList.getHead();
        while (fNode != nullptr) {
            foodFile << fNode->id << "," << fNode->name << "," << fNode->price << "\n";
            fNode = fNode->next;
        }
        foodFile.close();
    }
    else {
        cout << "Warning: Unable to update food.csv." << endl;
    }

    // 6b. Rewrite food_stall_map.csv
    ofstream mapFile("food_stall_map.csv", ios::out | ios::trunc);
    if (mapFile.is_open()) {
        mapFile << "food_id,stall_id\n"; // Header row
        struct food_stall_map* mNode = foodStallMapList.getHead();
        while (mNode != nullptr) {
            mapFile << mNode->food_id << "," << mNode->stall_id << "\n";
            mNode = mNode->next;
        }
        mapFile.close();
    }
    else {
        cout << "Warning: Unable to update food_stall_map.csv." << endl;
    }

    cout << "\n[SUCCESS] Menu item \"" << deletedName << "\" (ID: " << targetId
        << ") and its stall mappings were deleted successfully!" << endl;
}

void updateMenuItem() {

}

void searchMenuItem() {

}