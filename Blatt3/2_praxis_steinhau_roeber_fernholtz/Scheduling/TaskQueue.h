#include "../Tasks/Task.h"
#include "../Tasks/TaskPool.h"

typedef struct _Queue_Element
{
	Task* task;
	struct _Queue_Element* next;
	struct _Queue_Element* previous;
} Q_Elem;

typedef struct _Queue
{
	Q_Elem* head;
	Q_Elem* tail;
	int size;
} Queue;

void enqueue(Queue* queue, Task* task);

Task* dequeue(Queue* queue);

Task* getHead(Queue* queue);

void printQueue(Queue* queue);