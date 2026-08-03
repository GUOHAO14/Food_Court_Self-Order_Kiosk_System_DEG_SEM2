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

class Node {
    public:
        Order data;
        Node* next;

        Node(Order order)
        {
            data = order;
            next = nullptr;
        }
};

class LinkedList {
    private:
        Node* head;
        Node* tail;

    public:
        LinkedList()
        {
            head = nullptr;
            tail = nullptr;
        }

        // Destructor
        ~LinkedList()
        {
            while (!isEmpty())
            {
                dequeue();
            }
        }

        // Check whether queue is empty
        bool isEmpty()
        {
            return head == nullptr;
        }

        // Enqueue
        void enqueue(Order order)
        {
            Node* newNode = new Node(order);

            if (isEmpty())
            {
                head = tail = newNode;
            }
            else
            {
                tail->next = newNode;
                tail = newNode;
            }
        }

        // Dequeue
        void dequeue()
        {
            if (isEmpty())
            {
                cout << "Queue is empty.\n";
                return;
            }

            Node* temp = head;
            head = head->next;

            if (head == nullptr)
            {
                tail = nullptr;
            }

            delete temp;
        }

        // Peek front
        Order front()
        {
            if (isEmpty())
            {
                throw runtime_error("Queue is empty.");
            }

            return head->data;
        }

        // Peek rear
        Order rear()
        {
            if (isEmpty())
            {
                throw runtime_error("Queue is empty.");
            }

            return tail->data;
        }

        // Display queue
        void display()
        {
            if (isEmpty())
            {
                cout << "Queue is empty.\n";
                return;
            }

            Node* current = head;

            while (current != nullptr)
            {
                cout << "Order ID : " << current->data.getOrderID() << endl;
                cout << "Student  : " << current->data.getStudentID() << endl;
                cout << "Food     : " << current->data.getFoodItem() << endl;
                cout << "------------------------" << endl;

                current = current->next;
            }
        }

        // Number of nodes
        int size()
        {
            int count = 0;
            Node* current = head;

            while (current != nullptr)
            {
                count++;
                current = current->next;
            }

            return count;
        }
};