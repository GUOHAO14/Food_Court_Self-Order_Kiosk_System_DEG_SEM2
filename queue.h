#pragma once
#include <iostream>
#include "food.h"

using namespace std;

class Order {
    private:
        int OrID;
        int StuID;
        string orderStatus;
        string orderTime;
        Food_Linked_List foods;

    public:
        Order() 
        {
            OrID = 0;
            StuID = 0;
            orderTime = "";
            orderStatus = "";
        }

        Order(int OrID, int StuID, string orderTime, string orderStatus)
        {
            this->OrID = OrID;
            this->StuID = StuID;
            this->orderTime = orderTime;
            this->orderStatus = orderStatus;
        }

        void addFood(food* food)
        {
            foods.insertRear(food);
        }

		void displayOrder()
		{
			cout << "Order ID : " << OrID << endl;
			cout << "Student  : " << StuID << endl;
			cout << "Time     : " << orderTime << endl;
			cout << "Status   : " << orderStatus << endl;
            foods.displayAllFood();
            cout << "------------------------" << endl;
		}

		int getOrderID()
		{
			return OrID;
		}

        int getStudentID()
        {
            return StuID;
        }

        string getOrderTime()
        {
            return orderTime;
        }

        string getOrderStatus()
        {
            return orderStatus;
        }

		void setOrderStatus(string status)
		{
			orderStatus = status;
		}

		Food_Linked_List* getFoodList()
		{
			return &foods;
		}

        void updateOrderStatus()
        {
            if (this == nullptr)
            {
                cout << "Order ID " << this->OrID << " not found in the queue." << endl;
                return;
            }

            if (this->getFoodList()->getCount() == 0) {
                cout << "Order ID " << this->OrID << " does not have a food item." << endl;
                return;
            }

            //start to go through the food list of the order to count the status of each food item
            struct food* currentFood = this->getFoodList()->getHead();

            int pending = 0;
            int processing = 0; // pending and processing (in stall queue) are mixed
            int completed = 0;

            while (currentFood != nullptr) {
                string status = currentFood->status;

                if (status == "Pending") {
                    pending++;
                }
                else if (status == "Processing") {
                    processing++;
                }
                else if (status == "Completed") {
                    completed++;
                }

                currentFood = currentFood->next;
            }

            if (pending == 0 && processing == 0 && completed > 0) {
                this->orderStatus = "Completed";
            }
            else if (processing > 0) {
                this->orderStatus = "Processing";
            }
            else if (processing == 0 && completed == 0 && pending > 0) {
                this->orderStatus = "Pending";
            }
            else {
                this->orderStatus = "Pending";
            }


        }
};

struct Node {
    Order* data;
    Node* next;

    Node(Order* order)
    {
        data = order;
        next = nullptr;
    }
};

class Queue {
private:
    Node* head;
    Node* tail;

public:
    Queue()
    {
        head = nullptr;
        tail = nullptr;
    }

    Node* getHead() {
        return head;
    }

    bool emptyList()
    {
        return head == nullptr;
    }

    void addQueue(Order* order)
    {
        Node* newNode = new Node(order);

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

    Order* delQueue()
    {
        if (emptyList())
        {
            cout << "The queue is currently empty." << endl;
            return nullptr;
        }

        Node* temp = head;
        head = head->next;

        if (head == nullptr)
        {
            tail = nullptr;
        }

        Order* order = temp->data;
        delete temp;

        return order;
    }

    // used for processing queue
    // breaks the rule of queue's FIFO convention
    // BUT REQUIRED BY BUSINESS LOGIC: food processing speed by stalls differs
    // UNABLE TO FORCE FIFO FOR processing -> completed
    bool delQueue(int orderId)
    {
        if (emptyList()) {
            cout << "The queue is currently empty." << endl;
            return false;
        }

        Node* currentNode = head;
        Node* prevNode = nullptr;

        while (currentNode != nullptr) {
            if (currentNode->data->getOrderID() == orderId) {
                // deleting head
                if (prevNode == nullptr) {
                    head = currentNode->next;
                }
                else {
                    prevNode->next = currentNode->next;
                }

                // deleting tail
                if (currentNode == tail) {
                    tail = prevNode;
                }

                // queue becomes empty
                if (head == nullptr) {
                    tail = nullptr;
                }

                delete currentNode;
                return true;
            }

            prevNode = currentNode;
            currentNode = currentNode->next;
        }

        cout << "Order not found." << endl;
        return false;
    }

    Order* searchOrderById(int id)
    {
        Node* currentNode = head;

        while (currentNode != nullptr)
        {
            if (currentNode->data->getOrderID() == id)
            {
                return currentNode->data;
            }

            currentNode = currentNode->next;
        }

        return nullptr;
    }

    void displayQueue()
    {
        if (emptyList())
        {
            cout << "The queue is currently empty, nothing to display." << endl;
            return;
        }

        Node* currentNode = head;

        while (currentNode != nullptr)
        {
            currentNode->data->displayOrder();
            currentNode = currentNode->next;
        }
    }

    int queueNum()
    {
        int count = 0;
        Node* currentNode = head;

        while (currentNode != nullptr)
        {
            count++;
            currentNode = currentNode->next;
        }

        return count;
    }
};