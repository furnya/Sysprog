#include "MLF.h"
#include "TaskQueue.h"
#include "FCFS.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int getNextTask(Queue** queueArray, uint16_t num_levels){
	for(int i=0;i<num_levels;i++){
    	if(!isEmpty(queueArray[i])){
    		return i;
    	}
    }
    return -1;
}

void schedule_MLF(const TaskPool *task_pool, uint16_t num_levels) {
    // TODO
    if(task_pool->size <= 0 || task_pool->total_duration <= 0){
    	return;
    }

    if(num_levels==1){
    	schedule_FCFS(task_pool);
    	return;
    }

    Queue** queueArray = (Queue**) malloc(sizeof(Queue*) * num_levels);
    if(queueArray==NULL){
    	perror("Malloc Error");
    	exit(1);
    }
    for(int i=0;i<num_levels;i++){
    	queueArray[i] = (Queue*) malloc(sizeof(Queue));
    	if(queueArray[i]==NULL){
    		perror("Malloc Error");
    		exit(1);
    	}
        queueArray[i]->size = 0;
    }

    int ticks_executed = 0;
    int currentQueue = 0;
    Task* arrived;
    Task* current = NULL;
    for(int tick=0;tick<task_pool->total_duration;tick++){
    	arrived = checkArrivals(task_pool,tick);
        if(arrived != NULL){
    		enqueue(queueArray[0], arrived);
    	}
    	if(isDone(current)){
    		currentQueue = getNextTask(queueArray, num_levels);
    		current = dequeue(queueArray[currentQueue]);
    		ticks_executed = 0;
    	}else{
    		if(currentQueue < num_levels-1){
    			if(ticks_executed == pow(2,currentQueue)){
    				enqueue(queueArray[currentQueue+1],current);
    				currentQueue = getNextTask(queueArray, num_levels);
    				current = dequeue(queueArray[currentQueue]);
    				ticks_executed = 0;
    			}
    		}
    	}
    	execTask(current, 1);
    	ticks_executed++;
    }

    for(int i=0;i<num_levels;i++){
    	freeQueue(queueArray[i]);
    }
    free(queueArray);

    if(allDone(task_pool)){
        printf("All done\n");
    }else{
        printf("Not all done\n");
        exit(1);
    }
}
