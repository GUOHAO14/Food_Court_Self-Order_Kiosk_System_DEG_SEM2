#include <iostream>
#include <string>
#include "task1.hpp"

using namespace std;

int main()
{
    LinkedList queue;

    queue.enqueue(Order("O001", "TP076357", "Chicken Rice"));
    queue.enqueue(Order("O002", "TP076358", "Nasi Lemak"));
    queue.enqueue(Order("O003", "TP076359", "Burger"));

    cout << "Queue:\n";
    queue.display();

    cout << "\nFront Order: "
        << queue.front().getOrderID() << endl;

    cout << "Rear Order: "
        << queue.rear().getOrderID() << endl;

    cout << "\nDequeue one order...\n";
    queue.dequeue();

    queue.display();

    cout << "\nQueue Size: " << queue.size() << endl;

    return 0;
}