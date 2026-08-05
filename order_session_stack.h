#pragma once
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

class SessionStep {
private:
    string StepID;
    string StuID;
    string action;

    bool isItem;
    int foodId;
    string foodName;
    double price;
    int quantity;

public:
    SessionStep() {
        isItem = false;
        foodId = -1;
        price = 0.0;
        quantity = 0;
    }

    SessionStep(string StepID, string StuID, string action)
    {
        this->StepID = StepID;
        this->StuID = StuID;
        this->action = action;
        this->isItem = false;
        this->foodId = -1;
        this->price = 0.0;
        this->quantity = 0;
    }

    SessionStep(string StepID, string StuID, string action, int foodId, string foodName, double price, int quantity)
    {
        this->StepID = StepID;
        this->StuID = StuID;
        this->action = action;
        this->isItem = true;
        this->foodId = foodId;
        this->foodName = foodName;
        this->price = price;
        this->quantity = quantity;
    }

    string getStepID()
    {
        return StepID;
    }

    string getStudentID()
    {
        return StuID;
    }

    string getAction()
    {
        return action;
    }

    bool isItemStep()
    {
        return isItem;
    }

    int getFoodId()
    {
        return foodId;
    }

    string getFoodName()
    {
        return foodName;
    }

    double getPrice()
    {
        return price;
    }

    int getQuantity()
    {
        return quantity;
    }
};

struct StackNode {
    SessionStep data;
    StackNode* next;

    StackNode(SessionStep step)
    {
        data = step;
        next = nullptr;
    }
};

class Stack {
public:

    static const int MAX_HISTORY_SIZE = 100;

private:
    StackNode* top;
    int size;

    void buildCartSummaryRec(StackNode* node, int remaining, string& result)
    {
        if (node == nullptr || remaining == 0)
        {
            return;
        }

        buildCartSummaryRec(node->next, remaining - 1, result);

        if (!result.empty())
        {
            result += ", ";
        }
        result += node->data.getFoodName() + " x" + to_string(node->data.getQuantity());
    }

    void collectCartItemsRec(StackNode* node, int remaining, SessionStep outItems[], int& count)
    {
        if (node == nullptr || remaining == 0)
        {
            return;
        }

        collectCartItemsRec(node->next, remaining - 1, outItems, count);
        outItems[count] = node->data;
        count++;
    }

    bool isSessionBoundaryFor(StackNode* node, const string& studentId)
    {
        if (node->data.getStudentID() != studentId)
        {
            return false;
        }
        string action = node->data.getAction();
        return action.rfind("Order Placed", 0) == 0 || action.rfind("Cancelled Order", 0) == 0;
    }

public:
    Stack()
    {
        top = nullptr;
        size = 0;
    }

    bool emptyList()
    {
        return top == nullptr;
    }

    bool isFull()
    {
        return size >= MAX_HISTORY_SIZE;
    }

    bool pushStep(SessionStep step)
    {
        if (isFull())
        {
            cout << "Session history limit reached (" << MAX_HISTORY_SIZE << " steps). "
                << "This action could not be recorded. Please finish, undo, or edit an existing step before continuing." << endl;
            return false;
        }

        StackNode* newNode = new StackNode(step);
        newNode->next = top;
        top = newNode;
        size++;
        return true;
    }

    SessionStep popStep()
    {
        if (emptyList())
        {
            cout << "The session history is currently empty. Cannot navigate back any further." << endl;
            return SessionStep();
        }

        StackNode* temp = top;
        top = top->next;
        SessionStep step = temp->data;
        delete temp;
        size--;
        return step;
    }

    SessionStep peekStep()
    {
        if (emptyList())
        {
            cout << "The session history is currently empty." << endl;
            return SessionStep();
        }
        return top->data;
    }

    string peekCartSummary(int n)
    {
        string result = "";
        buildCartSummaryRec(top, n, result);
        return result;
    }

    int getCartItems(int n, SessionStep outItems[])
    {
        int count = 0;
        collectCartItemsRec(top, n, outItems, count);
        return count;
    }

    int countActiveCartItemsForStudent(string studentId)
    {
        int count = 0;
        StackNode* node = top;
        while (node != nullptr)
        {
            if (isSessionBoundaryFor(node, studentId))
            {
                break;
            }
            if (node->data.getStudentID() == studentId && node->data.isItemStep())
            {
                count++;
            }
            node = node->next;
        }
        return count;
    }

    int getActiveCartItemsForStudent(string studentId, SessionStep outItems[])
    {
        SessionStep temp[MAX_HISTORY_SIZE];
        int count = 0;
        StackNode* node = top;
        while (node != nullptr)
        {
            if (isSessionBoundaryFor(node, studentId))
            {
                break;
            }
            if (node->data.getStudentID() == studentId && node->data.isItemStep())
            {
                temp[count] = node->data;
                count++;
            }
            node = node->next;
        }

        for (int i = 0; i < count; i++)
        {
            outItems[i] = temp[count - 1 - i];
        }
        return count;
    }

    string peekCartSummaryForStudent(string studentId)
    {
        SessionStep items[MAX_HISTORY_SIZE];
        int count = getActiveCartItemsForStudent(studentId, items);

        string result = "";
        for (int i = 0; i < count; i++)
        {
            if (!result.empty())
            {
                result += ", ";
            }
            result += items[i].getFoodName() + " x" + to_string(items[i].getQuantity());
        }
        return result;
    }

    void displayHistory(string filterStudentId = "")
    {
        if (emptyList())
        {
            cout << "The session history is currently empty, nothing to display." << endl;
            return;
        }

        bool anyMatched = false;
        StackNode* currentNode = top;
        int position = stackSize();
        while (currentNode != nullptr)
        {
            bool matches = filterStudentId.empty() || currentNode->data.getStudentID() == filterStudentId;

            if (matches)
            {
                anyMatched = true;
                cout << "Step " << position << " (Step ID: " << currentNode->data.getStepID() << ")" << (currentNode == top ? "  <-- current" : "") << endl;
                cout << "Student : " << currentNode->data.getStudentID() << endl;
                cout << "Action  : " << currentNode->data.getAction() << endl;

                if (currentNode->data.isItemStep())
                {
                    cout << "Food ID : " << currentNode->data.getFoodId() << endl;
                    cout << "Food    : " << currentNode->data.getFoodName() << endl;
                    cout << "Price   : RM" << fixed << setprecision(2) << currentNode->data.getPrice() << endl;
                    cout << "Quantity: " << currentNode->data.getQuantity() << endl;
                }

                cout << "------------------------" << endl;
            }

            currentNode = currentNode->next;
            position--;
        }

        if (!anyMatched)
        {
            cout << "No session history found for Student ID " << filterStudentId << "." << endl;
        }
    }

    // Counts only the steps belonging to a given student ID.
    int stackSizeForStudent(string studentId)
    {
        int count = 0;
        StackNode* currentNode = top;
        while (currentNode != nullptr)
        {
            if (currentNode->data.getStudentID() == studentId)
            {
                count++;
            }
            currentNode = currentNode->next;
        }
        return count;
    }

    int stackSize()
    {
        return size;
    }
};

