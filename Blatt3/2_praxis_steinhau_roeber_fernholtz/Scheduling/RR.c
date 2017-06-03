#include "RR.h"
#include "TaskQueue.h"
#include <stdio.h>
#include <stdlib.h>

void schedule_RR(const TaskPool *task_pool, uint16_t quantum_max) {
    // TODO
    if(task_pool->size <= 0 || task_pool->total_duration <= 0){
    	return;
    }

    Queue* queue = (Queue*) malloc(sizeof(Queue));
    if(queue==NULL){
    	perror("Malloc Error");
    	exit(1);
    }
    queue->size = 0;
    Task* current = NULL;
    int tick = 0;
    Task* arrived = NULL;
    while(!allDone(task_pool)){
    	arrived = checkArrivals(task_pool,tick);
        if(arrived != NULL){
    		enqueue(queue, arrived);
    	}
    	if(!isDone(current)){
    		enqueue(queue, current);
    	}
    	current = dequeue(queue);
    	for(int i=0;i<quantum_max && !isDone(current);i++){
    		if(i!=0){
    			arrived = checkArrivals(task_pool,tick);
        		if(arrived != NULL){
    				enqueue(queue, arrived);
    			}
    		}
    		tick += execTask(current, 1);
    	}
    }

    freeQueue(queue);

    if(allDone(task_pool)){
        printf("All done\n");
    }else{
        printf("Not all done\n");
        exit(1);
    }
}
