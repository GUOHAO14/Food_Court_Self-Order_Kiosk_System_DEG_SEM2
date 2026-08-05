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

        void addFood(food food)
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

		Food_Linked_List* getFoodList()
		{
			return &foods;
		}
};

struct Node {
    Order data;
    Node* next;

    Node(Order order)
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

        bool emptyList()
        {
            return head == nullptr;
        }

        void addQueue(Order order)
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

        Order delQueue()
        {
            if (emptyList())
            {
                cout << "The queue is curerntly empty";
                return Order();
            }

            Node* temp = head;
            head = head->next;
            if (head == nullptr)
            {
                tail = nullptr;
            }
            Order order = temp->data;
            delete temp;
            return order;
        }

        Order* searchOrderById(int id) {
            Node* currentNode = head;
            while (currentNode != nullptr)
            {
                if (currentNode->data.getOrderID() == id) {
                    return &currentNode->data;
                }
                currentNode = currentNode->next;
            }
            return nullptr;
        }

        void displayQueue()
        {
            if (emptyList())
            {
                cout << "The queue is currently empty, nothing to display.";
                return;
            }

            Node* currentNode = head;
            while (currentNode != nullptr)
            {
                currentNode->data.displayOrder();
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

        void saveOrders(ofstream& orderOut, ofstream& mapOut)
        {
            Node* currentNode = head;
            while (currentNode != nullptr)
            {
                Order& order = currentNode->data;

                orderOut << order.getOrderID() << "," << order.getStudentID() << "," << order.getOrderTime() << "," << order.getOrderStatus() << endl;
                order.getFoodList()->saveOrderMapFood(mapOut, order.getOrderID());

                currentNode = currentNode->next;
            }
        }
};