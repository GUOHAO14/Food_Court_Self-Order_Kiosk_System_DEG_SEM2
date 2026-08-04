#pragma once
#include <iostream>

using namespace std;

struct FoodNode
{
    int foodID;
    FoodNode* next;

    FoodNode(int foodID)
    {
        this->foodID = foodID;
        next = nullptr;
    }
};

class order_map_foodList
{
private:
    FoodNode* head;
    FoodNode* tail;

public:

    order_map_foodList()
    {
        head = nullptr;
        tail = nullptr;
    }

    bool emptyList()
    {
        return head == nullptr;
    }

    void addFood(int foodID) 
    {
        FoodNode* newNode = new FoodNode(foodID);
        if (emptyList())
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
    }

    void displayFood() 
    {
        if (emptyList())
        {
            cout << "No food items, nothing to display.";
            return;
        }

        FoodNode* currentNode = head;
        while (currentNode != nullptr)
        {
            cout << "Food ID : " << currentNode->foodID << endl;
            currentNode = currentNode->next;
        }
    }

    int foodNum()
    {
        int count = 0;
        FoodNode* currentNode = head;
        while (currentNode != nullptr)
        {
            count++;
            currentNode = currentNode->next;
        }
        return count;
    }
};