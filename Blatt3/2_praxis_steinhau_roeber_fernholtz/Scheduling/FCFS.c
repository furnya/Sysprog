#include "FCFS.h"
#include "TaskQueue.h"
#include <stdio.h>
#include <stdlib.h>

void schedule_FCFS(const TaskPool *task_pool) {
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
    int tick = 0;
    Task* current;
    Task* arrived;
    bool done = true;
    while(!allDone(task_pool) && tick < task_pool->total_duration){
    	arrived = checkArrivals(task_pool,tick);
    	if(arrived != NULL){
    		enqueue(queue,arrived);
    	}
    	if(done){
    		current = dequeue(queue);
    		done = false;
    	}
    	execTask(current,1);
    	if(isDone(current)){
    		done = true;
    	}
    	tick++;
    }

    freeQueue(queue);

    if(allDone(task_pool)){
        printf("All done\n");
    }else{
        printf("Not all done\n");
        exit(1);
    }
}
