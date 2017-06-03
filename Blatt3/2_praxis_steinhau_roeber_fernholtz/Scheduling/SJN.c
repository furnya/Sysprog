#include "SJN.h"
#include "TaskQueue.h"
#include <stdio.h>
#include <stdlib.h>

Task* find_shortest(Task** array, int arraysize){
    Task* task;
    int taskIndex;
	bool foundOne = false;
	for(int i=0;i<arraysize;i++){
		if(array[i]==NULL){
            continue;
        }
        if(!foundOne){
            task = array[i];
            taskIndex = i;
            foundOne = true;
        }else{
            if(array[i]->total_ticks < task->total_ticks){
                taskIndex = i;
                task = array[i];
            }
        }
	}
    array[taskIndex] = NULL;
	return task;
}

void schedule_SJN(const TaskPool *task_pool) {
    // TODO
    if(task_pool->size <= 0 || task_pool->total_duration <= 0){
    	return;
    }

    Task** array = (Task**) malloc(sizeof(Task*) * task_pool->size);
    for(int i = 0;i<task_pool->size;i++){
        array[i] = NULL;
    }
    Task* arrived;
    int arraysize = 0;
    Task* current = NULL;
    for(int tick = 0;tick<task_pool->total_duration;tick++){
    	arrived = checkArrivals(task_pool,tick);
        if(arrived != NULL){
            array[arraysize] = arrived;
            arraysize++;
        }
    	if(isDone(current)){
    		current = find_shortest(array, arraysize);
    	}
    	execTask(current,1);
    }

    if(allDone(task_pool)){
        printf("All done\n");
    }else{
        printf("Not all done\n");
        exit(1);
    }
}