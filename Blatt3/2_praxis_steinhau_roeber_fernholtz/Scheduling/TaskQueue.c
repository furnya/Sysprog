#include "../Tasks/Task.h"
#include "../Tasks/TaskPool.h"
#include "TaskQueue.h"
#include <stdio.h>
#include <stdlib.h>

void enqueue(Queue* queue, Task* task){
	//printf("enqueue | arrival_tick = %d\n",task->arrival_tick);
	Q_Elem* new_elem = (Q_Elem*) malloc(sizeof(Q_Elem));
	if(new_elem==NULL){
		perror("Malloc Error");
		exit(1);
	}
	new_elem->task = task;
	new_elem->previous = NULL;
	new_elem->next = queue->tail;
	if(queue->size==0){
		queue->head = new_elem;
	}else{
		queue->tail->previous = new_elem;
	}
	queue->tail = new_elem;
	queue->size++;
}

Task* dequeue(Queue* queue){
	if(queue->size==0){
		return NULL;
	}
	Q_Elem* elem = queue->head;
	Task* task = elem->task;
	queue->head = elem->previous;
	free(elem);
	queue->size--;
	return task;
}

Task* getHead(Queue* queue){
	if(queue->size==0){
		return NULL;
	}
	return queue->head->task;
}

void printQueue(Queue* queue){
	if(queue->size==0){
		return;
	}
	Q_Elem* temp = queue->head;
	printf("Queue: ");
	while(temp!=NULL){
		printTask(temp->task);
		printf(" | ");
		temp = temp->previous;
	}
	printf("\n");
}