#include "../Tasks/Task.h"
#include "../Tasks/TaskPool.h"



typedef struct _Queue_Element
{
	Task* task;							//Pointer auf einen Task
	struct _Queue_Element* next;		//Pointer auf das nächste Element in der Queue
	struct _Queue_Element* previous;	//Pointer auf das vorige ELement in der Queue
} Q_Elem;

typedef struct _Queue
{
	Q_Elem* head;			//Pointer auf den Kopf der Queue
	Q_Elem* tail;			//Pointer auf das Ende der Queue
	int size;				//Größe der Queue
} Queue;

/*
*	@brief 	Erstellt ein neues Queue-Element, speichert einen Pointer auf den übergebenen Task darin und reiht es in die Queue ein
*	@param	queue	Die Queue, in die das Element eingereiht werden soll
*	@param	task	Ein Pointer auf den Task, der im Queue-Element gespeichert werden soll
*/
void enqueue(Queue* queue, Task* task);

/*
*	@brief 	Löscht den Kopf der Queue und gibt den zugehörigen Task zurück
*	@param	Die Queue, aus der der Kopf gelöscht werden soll
*	@return	Der Task, der im Kopf-Element gespeichert war (falls die Queue leer ist -> NULL)
*/
Task* dequeue(Queue* queue);

/*
*	@brief 	Gibt den Task des Queue-Kopfes zurück
*	@param	Die Queue, aus der der Kopf-Task zurückgegeben werden soll
*	@return	Der Task, der im Kopf-Element gespeichert ist (falls die Queue leer ist -> NULL)
*/
Task* getHead(Queue* queue);

/*
* Ignorieren, nur zum debuggen benutzt
*/
void printQueue(Queue* queue);

/*
*	@brief	Testet, ob die Queue leer ist
*	@param	queue	Die Queue, die getestet werden soll
*	@return	true bei leerer Queue, false bei nicht-leerer
*/
bool isEmpty(Queue* queue);

/*
*	@brief Löscht die Queue-Elemente, die Queue und gibt den Speicher frei
*	@param queue	Die Queue, die gelöscht werden soll
*/
void freeQueue(Queue* queue);