#pragma once
#pragma once
#include "order_session_stack.h"
#include "food.h"
#include "queue.h"

// Runs the interactive self-order kiosk flow: scans student ID, lets the
// student browse/select/undo/edit items, records every step into the session
// history (Stack), then builds and enqueues an Order once the student
// finishes. Returns a pointer to the Order that was added to the pending
// queue, or nullptr if the order was cancelled (no items selected).
Order* orderInterface(Queue* pendingQueuePtr, Food_Linked_List* foodListPtr, Stack* sessionHistoryPtr);

int buildOrderId();
string formatOrderId(int id);
