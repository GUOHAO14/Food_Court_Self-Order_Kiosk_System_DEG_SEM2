#include <iostream>
#include <string>
#include "food.h"

using namespace std;

class Order {
    private:
        int OrID;
        int StuID;
        string orderTime;
        Food_Linked_List foods;

    public:
        Order() 
        {
            OrID = 0;
            StuID = 0;
            orderTime = "";
        }

        Order(int OrID, int StuID, string orderTime)
        {
            this->OrID = OrID;
            this->StuID = StuID;
            this->orderTime = orderTime;
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

        void addFood(food food)
        {
            foods.insertRear(food);
        }

        Food_Linked_List& getFoodList()
        {
            return foods;
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
                cout << "Order ID : " << currentNode->data.getOrderID() << endl;
                cout << "Student  : " << currentNode->data.getStudentID() << endl;
                cout << "Time     : " << currentNode->data.getOrderTime() << endl;
                currentNode->data.getFoodList().displayAllFood();
                cout << "------------------------" << endl;
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