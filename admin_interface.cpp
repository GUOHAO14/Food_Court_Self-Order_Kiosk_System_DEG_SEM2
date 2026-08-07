#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "globals.h"
#include "admin.h"
#include "stall.h"
#include "food.h"
#include "food_stall_map.h"

using namespace std;

// Global stall selection pointer
extern struct stall* selectedStall;


// Used to remove residual characters
static void clearInputBuffer() {
    cin.clear();
    cin.ignore(10000, '\n');
}

// Reusable function to display all the menu items
void displayAllMenuItems() {
    struct food* head = foodList.getHead();

    if (head == nullptr) {
        cout << "\nThere are currently no menu items available." << endl;
        return;
    }

    int length = 75;
    cout << string(length, '=') << endl;
    cout << "| " << left << setw(8) << "Food ID"
        << " | " << left << setw(28) << "Food Name"
        << " | " << left << setw(10) << "Price (RM)"
        << " | " << left << setw(18) << "Assigned Stall ID(s)" << " |" << endl;
    cout << string(length, '=') << endl;

    struct food* currentFood = head;
    while (currentFood != nullptr) {
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
}

// Reusable function to display all the available stalls
void displayAvailableStalls() {
    cout << "\n---------------- Available Stalls ----------------" << endl;
    cout << left << setw(10) << "Stall ID" << " | " << "Stall Name" << endl;
    cout << "--------------------------------------------------" << endl;

    struct stall* currentStall = stallList.getHead();
    while (currentStall != nullptr) {
        cout << left << setw(10) << currentStall->id << " | " << currentStall->name << endl;
        currentStall = currentStall->next;
    }
    cout << "--------------------------------------------------" << endl;
}

// Update the csv files
void rewriteCSV() {
    
    // food.csv
    ofstream foodFile("food.csv", ios::out | ios::trunc);
    if (foodFile.is_open()) {
        foodFile << "food_id,food_name,price\n";
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

    // food_stall_map.csv
    ofstream mapFile("food_stall_map.csv", ios::out | ios::trunc);
    if (mapFile.is_open()) {
        mapFile << "food_id,stall_id\n";
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
}

void printSearchTableHeader(int length = 75) {
    cout << "\nSearch Results:" << endl;
    cout << string(length, '=') << endl;
    cout << "| " << left << setw(8) << "Food ID"
        << " | " << left << setw(28) << "Food Name"
        << " | " << left << setw(10) << "Price (RM)"
        << " | " << left << setw(18) << "Assigned Stall ID(s)" << " |" << endl;
    cout << string(length, '=') << endl;
}

//Load Admin Page
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

        if (!(cin >> choice)) {
            cout << "Invalid input" << endl;
            clearInputBuffer();
            continue;
        }

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
            cout << "Invalid choice" << endl;
        }

    } while (choice != 5);
}

void addMenuItem() {
    string name;
    double price;

    cout << "\n================ Add Menu Item ================" << endl;

    clearInputBuffer();

    //Food Name
    while (true) {
        cout << "Enter food name (or type 'c' to cancel): ";
        getline(cin, name);

        if (name == "c" || name == "C") {
            cout << "Operation cancelled" << endl;
            return;
        }

        if (name.empty()) {
            cout << "Food name cannot be empty" << endl;
            continue;
        }

        if (foodList.isFoodNameExists(name)) {
            cout << "A menu item with the name \"" << name << "\" already exists.\n" << endl;
            continue;
        }
        else {
            break;
        }
    }

    //Food Price
    cout << "Enter food price (RM) [or type 0 to cancel]: ";
    while (!(cin >> price) || price < 0) {
        cout << "Invalid price (or 0 to cancel): ";
        clearInputBuffer();
    }

    if (price == 0) {
        cout << "Operation cancelled" << endl;
        return;
    }

    // Auto-generate Food ID
    int maxId = 0;
    struct food* currentFood = foodList.getHead();
    while (currentFood != nullptr) {
        if (currentFood->id > maxId) {
            maxId = currentFood->id;
        }
        currentFood = currentFood->next;
    }
    int newFoodId = maxId + 1;

    // Assign Stall
    if (stallList.getHead() == nullptr) {
        cout << "No stalls available." << endl;
        return;
    }

    displayAvailableStalls();

    int assignedCount = 0;
    string input;

    while (true) {
        if (assignedCount == 0) {
            cout << "\nEnter Stall ID to assign (or 'c' to cancel): ";
        }
        else {
            cout << "\nEnter Stall ID to assign ('s' to save & finish, 'c' to cancel): ";
        }

        cin >> input;

        // Process single-character controls
        if (tolower(input[0]) == 'c' && input.length() == 1) {
            cout << "Menu creation cancelled." << endl;
            if (assignedCount > 0) {
                foodStallMapList.deleteByFoodId(newFoodId); // Rollback mappings
            }
            return;
        }

        if (assignedCount > 0 && tolower(input[0]) == 's' && input.length() == 1) {
            cout << "Stall assignment completed." << endl;
            break;
        }

        // Convert string to numeric ID safely
        int stallId = 0;
        try {
            stallId = stoi(input);
        }
        catch (...) {
            cout << "Invalid input! Please enter a valid Stall ID or command." << endl;
            continue;
        }

        // Validate Stall Existence
        if (stallList.searchStallById(stallId) == nullptr) {
            cout << "Error: Stall ID " << stallId << " does not exist." << endl;
            continue;
        }

        // Check for duplicates
        if (foodStallMapList.checkFoodStallMapping(newFoodId, stallId)) {
            cout << "Food item is already assigned to Stall ID " << stallId << "." << endl;
            continue;
        }

        // Add mapping
        foodStallMapList.insertRear(newFoodId, stallId);
        assignedCount++;
        cout << "Assigned to Stall ID " << stallId << " successfully." << endl;

        // Ask whether to continue adding stalls
        char choice;
        cout << "Assign another stall? (y/n/c): ";
        cin >> choice;
        choice = tolower(choice);

        if (choice == 'c') {
            cout << "Menu creation cancelled." << endl;
            foodStallMapList.deleteByFoodId(newFoodId); // Rollback mappings
            return;
        }

        if (choice == 'n') {
            cout << "Stall assignment completed." << endl;
            break;
        }

    } 

    if (assignedCount == 0) {
        cout << "No stalls assigned. Menu item creation aborted." << endl;
        return;
    }

    //Update csv
    foodList.insertRear(newFoodId, name, price);
    rewriteCSV();

    cout << "\n[SUCCESS] Menu item \"" << name << "\" (ID: " << newFoodId
        << ") added and assigned to " << assignedCount << " stall(s)!" << endl;
}

void removeMenuItem() {
    if (foodList.getHead() == nullptr) {
        cout << "\nThere are currently no menu items available to remove." << endl;
        return;
    }

    cout << "\n================ Remove Menu Item ================" << endl;

    // Display menu table using shared helper
    displayAllMenuItems();

    int targetId;
    cout << "\nEnter Food ID to delete (or type 0 to cancel): ";
    if (!(cin >> targetId) || targetId <= 0) {
        if (targetId == 0) {
            cout << "Operation cancelled." << endl;
        }
        else {
            cout << "Invalid input." << endl;
            clearInputBuffer();
        }
        return;
    }

    struct food* targetFood = foodList.searchFoodById(targetId);
    if (targetFood == nullptr) {
        cout << "Error: Food ID " << targetId << " does not exist." << endl;
        return;
    }

    string deletedName = targetFood->name;

    char confirm;
    cout << "Are you sure you want to delete \"" << deletedName << "\" (ID: " << targetId << ")? (y/n): ";
    cin >> confirm;
    if (tolower(confirm) != 'y') {
        cout << "Deletion cancelled." << endl;
        return;
    }

    // Delete from linked lists
    foodList.deleteFoodById(targetId);
    foodStallMapList.deleteByFoodId(targetId);

    // Save updated state to files
    rewriteCSV();

    cout << "\n[SUCCESS] Menu item \"" << deletedName << "\" (ID: " << targetId
        << ") and its stall mappings were deleted successfully!" << endl;
}

void updateMenuItem() {
    if (foodList.getHead() == nullptr) {
        cout << "\nThere are currently no menu items available to update." << endl;
        return;
    }

    cout << "\n================ Update Menu Item ================" << endl;

    // Display menu table
    displayAllMenuItems();

    int targetId;
    cout << "\nEnter Food ID to update (or type 0 to cancel): ";
    if (!(cin >> targetId) || targetId <= 0) {
        if (targetId == 0) {
            cout << "Operation cancelled." << endl;
        }
        else {
            cout << "Invalid input." << endl;
            clearInputBuffer();
        }
        return;
    }

    struct food* targetFood = foodList.searchFoodById(targetId);
    if (targetFood == nullptr) {
        cout << "Error: Food ID " << targetId << " does not exist." << endl;
        return;
    }

    int updateChoice = 0;
    cout << "\nWhat would you like to update for \"" << targetFood->name << "\" (ID: " << targetId << ")?" << endl;
    cout << "1. Update Food Name" << endl;
    cout << "2. Update Price" << endl;
    cout << "3. Re-assign Stalls" << endl;
    cout << "4. Update All Details" << endl;
    cout << "5. Cancel" << endl;
    cout << "Enter choice (1-5): ";

    if (!(cin >> updateChoice) || updateChoice < 1 || updateChoice > 5) {
        cout << "Invalid selection or operation cancelled." << endl;
        clearInputBuffer();
        return;
    }

    if (updateChoice == 5) {
        cout << "Update cancelled." << endl;
        return;
    }

    clearInputBuffer();
    bool modified = false;

    // Update name
    if (updateChoice == 1 || updateChoice == 4) {
        string newName;
        while (true) {
            cout << "Enter new food name (Current: " << targetFood->name << ") [Leave it empty to keep current]: ";
            getline(cin, newName);

            if (newName.empty() || newName == targetFood->name) {
                cout << "Food name left unchanged." << endl;
                break;
            }

            if (foodList.isFoodNameExists(newName)) {
                cout << "Error: Menu item \"" << newName << "\" already exists. Choose a different name." << endl;
            }
            else {
                targetFood->name = newName;
                modified = true;
                cout << "Food name updated successfully!" << endl;
                break;
            }
        }
    }

    // Update price
    if (updateChoice == 2 || updateChoice == 4) {
        double newPrice;
        cout << "Enter new price RM (Current: " << fixed << setprecision(2) << targetFood->price << ") [or 0 to keep current]: ";
        while (!(cin >> newPrice) || newPrice < 0) {
            cout << "Invalid price. Please enter a valid positive number (or 0 to keep current): ";
            clearInputBuffer();
        }

        if (newPrice > 0) {
            targetFood->price = newPrice;
            modified = true;
            cout << "Price updated successfully!" << endl;
        }
        else {
            cout << "Price left unchanged." << endl;
        }
    }

    // Re-assign Stalls
    if (updateChoice == 3 || updateChoice == 4) {
        if (stallList.getHead() == nullptr) {
            cout << "No stalls available to assign." << endl;
        }
        else {
            int origCount = 0;
            struct food_stall_map* mNode = foodStallMapList.getHead();
            while (mNode != nullptr) {
                if (mNode->food_id == targetId) {
                    origCount++;
                }
                mNode = mNode->next;
            }

            int* originalStalls = nullptr;
            if (origCount > 0) {
                originalStalls = new int[origCount];
                int idx = 0;
                mNode = foodStallMapList.getHead();
                while (mNode != nullptr) {
                    if (mNode->food_id == targetId) {
                        originalStalls[idx++] = mNode->stall_id;
                    }
                    mNode = mNode->next;
                }
            }

            displayAvailableStalls();

            foodStallMapList.deleteByFoodId(targetId);

            int assignedCount = 0;
            string input;

            while (true) {
                if (assignedCount == 0) {
                    cout << "\nEnter Stall ID to assign (or 'c' to cancel re-assignment): ";
                }
                else {
                    cout << "\nEnter Stall ID to assign ('s' to save & finish, 'c' to cancel re-assignment): ";
                }

                cin >> input;

                if (tolower(input[0]) == 'c') {
                    cout << "Stall re-assignment cancelled." << endl;

                    foodStallMapList.deleteByFoodId(targetId);

                    for (int i = 0; i < origCount; i++) {
                        foodStallMapList.insertRear(targetId, originalStalls[i]);
                    }

                    assignedCount = 0;
                    break;
                }

                if (tolower(input[0]) == 's') {
                    if (assignedCount == 0) {
                        cout << "Error: A menu item must be assigned to at least one stall before saving!" << endl;
                        continue;
                    }
                    cout << "Stall assignment completed." << endl;
                    break;
                }

                int stallId = 0;
                try {
                    stallId = stoi(input);
                }
                catch (...) {
                    cout << "Invalid input" << endl;
                    continue;
                }

                if (stallList.searchStallById(stallId) == nullptr) {
                    cout << "Error: Stall ID " << stallId << " does not exist." << endl;
                    continue;
                }

                if (foodStallMapList.checkFoodStallMapping(targetId, stallId)) {
                    cout << "Food item is already assigned to Stall ID " << stallId << "." << endl;
                    continue;
                }

                foodStallMapList.insertRear(targetId, stallId);
                assignedCount++;
                cout << "Assigned to Stall ID " << stallId << " successfully." << endl;
            }

            delete[] originalStalls;

            if (assignedCount > 0) {
                modified = true;
            }
        }
    }

    // Save changes if modified
    if (modified) {
        rewriteCSV();
        cout << "\n[SUCCESS] Menu item ID " << targetId << " updated successfully!" << endl;
    }
    else {
        cout << "\nNo changes were made to the menu item." << endl;
    }
}

void searchMenuItem() {
    if (foodList.getHead() == nullptr) {
        cout << "\nThere are currently no menu items to search." << endl;
        return;
    }

    // Populate BST from existing linked list
    FoodBST foodTree;
    foodTree.populateFromLinkedList(foodList.getHead());

    cout << "\n================ Search Menu Item (BST) ================" << endl;
    cout << "1. Search by Food ID" << endl;
    cout << "2. Search by Food Name" << endl;
    cout << "3. Search by Stall ID" << endl;
    cout << "4. Search by Stall Name" << endl;
    cout << "Enter search mode choice (1-4): ";

    int mode;
    if (!(cin >> mode) || mode < 1 || mode > 4) {
        cout << "Invalid choice. Returning to admin menu..." << endl;
        clearInputBuffer();
        return;
    }

    clearInputBuffer();

    int matchCount = 0;
    int length = 75;

    if (mode == 1) {
        int searchId;
        cout << "Enter Food ID to search: ";
        if (!(cin >> searchId)) {
            cout << "Invalid ID entered." << endl;
            clearInputBuffer();
            return;
        }

        printSearchTableHeader(length);

        FoodTreeNode* result = foodTree.searchById(searchId);
        if (result != nullptr) {
            matchCount = 1;
            FoodBST::displayNodeRow(result);
        }
    }
    else if (mode == 2) {
        string searchKeyword;
        cout << "Enter Food Name (or partial name): ";
        getline(cin, searchKeyword);

        if (searchKeyword.empty()) {
            cout << "Search keyword cannot be empty." << endl;
            return;
        }

        printSearchTableHeader(length);

        transform(searchKeyword.begin(), searchKeyword.end(), searchKeyword.begin(), ::tolower);
        foodTree.searchByName(searchKeyword, matchCount);
    }
    else if (mode == 3) {
        int stallId;
        cout << "Enter Stall ID to search assigned food items: ";
        if (!(cin >> stallId)) {
            cout << "Invalid Stall ID entered." << endl;
            clearInputBuffer();
            return;
        }

        printSearchTableHeader(length);

        foodTree.searchByStallId(stallId, matchCount);
    }
    else if (mode == 4) {
        string stallNameKeyword;
        cout << "Enter Stall Name (or partial stall name): ";
        getline(cin, stallNameKeyword);

        if (stallNameKeyword.empty()) {
            cout << "Search keyword cannot be empty." << endl;
            return;
        }

        printSearchTableHeader(length);

        transform(stallNameKeyword.begin(), stallNameKeyword.end(), stallNameKeyword.begin(), ::tolower);
        foodTree.searchByStallName(stallNameKeyword, matchCount);
    }

    cout << string(length, '=') << endl;

    if (matchCount == 0) {
        cout << "No matching menu items were found." << endl;
    }
    else {
        cout << "Found " << matchCount << " matching item(s)." << endl;
    }
}