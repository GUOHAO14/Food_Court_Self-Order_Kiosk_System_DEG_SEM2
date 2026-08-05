#pragma once

#include "stall.h"
#include "food.h"
#include "food_stall_map.h"

void adminPage(); // <-- ADD THIS LINE

void addMenuItem();
void removeMenuItem();
void updateMenuItem();
void searchMenuItem();

struct FoodTreeNode {
    int id;
    string name;
    double price;
    FoodTreeNode* left;
    FoodTreeNode* right;

    FoodTreeNode(int foodId, string foodName, double foodPrice)
        : id(foodId), name(foodName), price(foodPrice), left(nullptr), right(nullptr) {
    }
};

// 2. BST Class Wrapper
class FoodBST {
private:
    FoodTreeNode* root;

    // Helper: Insert node into BST based on Food ID
    FoodTreeNode* insertRecursive(FoodTreeNode* node, int id, const string& name, double price) {
        if (node == nullptr) {
            return new FoodTreeNode(id, name, price);
        }
        if (id < node->id) {
            node->left = insertRecursive(node->left, id, name, price);
        }
        else if (id > node->id) {
            node->right = insertRecursive(node->right, id, name, price);
        }
        return node;
    }

    // Helper: Clean up memory recursively
    void clearTree(FoodTreeNode* node) {
        if (node != nullptr) {
            clearTree(node->left);
            clearTree(node->right);
            delete node;
        }
    }

    // Helper: In-order traversal to search by partial name
    void searchByNameRecursive(FoodTreeNode* node, const string& searchKeyword, int& matchCount) {
        if (node == nullptr) return;

        searchByNameRecursive(node->left, searchKeyword, matchCount);

        // Case-insensitive name match check
        string foodNameLower = node->name;
        transform(foodNameLower.begin(), foodNameLower.end(), foodNameLower.begin(), ::tolower);

        if (foodNameLower.find(searchKeyword) != string::npos) {
            matchCount++;
            displayNodeRow(node);
        }

        searchByNameRecursive(node->right, searchKeyword, matchCount);
    }

public:
    FoodBST() : root(nullptr) {}
    ~FoodBST() { clearTree(root); }

    // Convert linked list -> BST
    void populateFromLinkedList(struct food* head) {
        clearTree(root);
        root = nullptr;

        struct food* current = head;
        while (current != nullptr) {
            root = insertRecursive(root, current->id, current->name, current->price);
            current = current->next;
        }
    }

    // O(log n) BST Lookup by ID
    FoodTreeNode* searchById(int id) {
        FoodTreeNode* current = root;
        while (current != nullptr) {
            if (id == current->id) {
                return current; // Found
            }
            if (id < current->id) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }
        return nullptr; // Not found
    }

    // Search by partial name
    void searchByName(const string& searchKeyword, int& matchCount) {
        searchByNameRecursive(root, searchKeyword, matchCount);
    }

    // Display formatted row for a single node
    static void displayNodeRow(FoodTreeNode* node) {
        string stallIdsStr = "";
        struct food_stall_map* currentMap = foodStallMapList.getHead();
        while (currentMap != nullptr) {
            if (currentMap->food_id == node->id) {
                if (!stallIdsStr.empty()) stallIdsStr += ", ";
                stallIdsStr += to_string(currentMap->stall_id);
            }
            currentMap = currentMap->next;
        }
        if (stallIdsStr.empty()) stallIdsStr = "None";

        cout << "| " << left << setw(8) << node->id
            << " | " << left << setw(28) << node->name
            << " | " << left << setw(10) << fixed << setprecision(2) << node->price
            << " | " << left << setw(20) << stallIdsStr << " |" << endl;
    }
};