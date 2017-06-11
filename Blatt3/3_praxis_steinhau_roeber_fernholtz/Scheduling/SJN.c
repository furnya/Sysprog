#include "SJN.h"
#include "TaskQueue.h"
#include <stdio.h>
#include <stdlib.h>


//Findet im Task-Array den Task mit der kürzesten Bedienzeit, löscht den Pointer auf diesen aus dem Array und gibt ihn zurück
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
    //Falls der Task-Pool leer ist, oder die Länge aller Prozesse nicht mindestens 1 ist, tue nichts
    if(task_pool->size <= 0 || task_pool->total_duration <= 0){
    	return;
    }

    //Erstelle ein Task-Pointer-Array
    Task** array = (Task**) malloc(sizeof(Task*) * task_pool->size);
    for(int i = 0;i<task_pool->size;i++){
        array[i] = NULL;
    }
    int arraysize = 0;

    //arrived wird verwendet, um einen Task, der im momentanen Tick ankommt (arrive), zwischenzuspeichern, bevor er im Array gespeichert wird
    Task* arrived;

    //Current ist der Task, der momentan ausgeführt wird
    Task* current = NULL;

    //Führe die Schleife so oft aus, wie lange alle Tasks im Task-Pool zusammengenommen zur Ausführung brauchen
    for(int tick = 0;tick<task_pool->total_duration;tick++){

        //Prüfe, ob in diesem Tick ein Task ankommt. Speicher diesen in arrived
    	arrived = checkArrivals(task_pool,tick);

        //Falls ein Task angekommen ist (!=NULL), speicher ihn im Array und erhöhe die Arraygröße
        if(arrived != NULL){
            array[arraysize] = arrived;
            arraysize++;
        }

        //Falls der vorher ausgeführte Task fertig ist, finde den kürzesten Task aus den bisher angekommenen Tasks (mit find_shortest) und setze ihn als neuen current-Task
    	if(isDone(current)){
    		current = find_shortest(array, arraysize);
    	}

        //Führe den current-Task für einen Tick aus
    	execTask(current,1);
    }

    //Lösche das Array
    free(array);
}