#include <iostream>
#include <string>

using namespace std;

class Order {
    private:
        string OrID;
        string StuID;
        string foods;

    public:
        Order() {}

        Order(string OrID, string StuID, string foods)
        {
            this->OrID = OrID;
            this->StuID = StuID;
            this->foods = foods;
        }

        string getOrderID()
        {
            return OrID;
        }

        string getStudentID()
        {
            return StuID;
        }

        string getFoodItem()
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
                cout << "Food     : " << currentNode->data.getFoodItem() << endl;
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