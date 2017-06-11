#include "MLF.h"
#include "TaskQueue.h"
#include "FCFS.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Gibt den Index der ersten Queue im Queue-Array zurück, die nicht leer ist (falls alle leer sind -> -1)
int getNextTask(Queue** queueArray, uint16_t num_levels){
	for(int i=0;i<num_levels;i++){
    	if(!isEmpty(queueArray[i])){
    		return i;
    	}
    }
    return -1;
}

void schedule_MLF(const TaskPool *task_pool, uint16_t num_levels) {
    //Falls der Task-Pool leer ist, oder die Länge aller Prozesse nicht mindestens 1 ist, tue nichts
    if(task_pool->size <= 0 || task_pool->total_duration <= 0){
    	return;
    }

    //Falls die vorgegebene Anhahl an Queues 1 ist, werden die Tasks einfach in FirstComeFirstServed ausgeführt
    if(num_levels==1){
    	schedule_FCFS(task_pool);
    	return;
    }

    //Erstelle ein Array aus Queues (Anzahl: num_levels)
    Queue** queueArray = (Queue**) malloc(sizeof(Queue*) * num_levels);
    if(queueArray==NULL){
    	perror("Malloc Error");
    	exit(1);
    }

    //Reserviere Speicher für jede Queue im Queue-Array
    for(int i=0;i<num_levels;i++){
    	queueArray[i] = (Queue*) malloc(sizeof(Queue));
    	if(queueArray[i]==NULL){
    		perror("Malloc Error");
    		exit(1);
    	}
        queueArray[i]->size = 0;
    }

    //Ticks executed zählt, wie viele Ticks der aktuelle Task ausgeführt wurde, seit er in der CPU ist
    int ticks_executed = 0;

    //currentQueue speichert den Index der Queue, aus der der aktuell ausgeführte Task kommt
    int currentQueue = 0;

    //arrived wird verwendet, um einen Task, der im momentanen Tick ankommt (arrive), zwischenzuspeichern, bevor er in die erste Queue eingereiht wird
    Task* arrived;

    //Current ist der Task, der momentan ausgeführt wird
    Task* current = NULL;

    //Führe die Schleife so oft aus, wie lange alle Tasks im Task-Pool zusammengenommen zur Ausführung brauchen
    for(int tick=0;tick<task_pool->total_duration;tick++){

        //Prüfe, ob in diesem Tick ein Task ankommt. Speicher diesen in arrived
    	arrived = checkArrivals(task_pool,tick);

        //Falls ein Task angekommen ist (!=NULL), reihe ihn in die erste Queue ein
        if(arrived != NULL){
    		enqueue(queueArray[0], arrived);
    	}

        //Falls der zuletzt ausgeführte Task fertig ist, hole einen neuen Task aus dem Queue-Array
    	if(isDone(current)){
            //speichere den Index der ersten nichtleeren Queue in currentQueue
    		currentQueue = getNextTask(queueArray, num_levels);
            //Hole den ersten Task aus der Queue mit dem Index currentQueue
    		current = dequeue(queueArray[currentQueue]);
    		ticks_executed = 0;
        //Falls der zuletzt ausgeführte Task nicht fertig ist:
    	}else{
            //Falls die momentane Queue nicht die letzte (nicht FCFS) ist:
    		if(currentQueue < num_levels-1){
                //Falls der current-Task seit er in der CPU ist 2^(Queue-Index) Ticks ausgeführt wurde:
                //- Reihe den current-Task in die Queue ein
                //- Hole einen neuen Task aus der ersten nicht-leeren Queue im Queue-Array (mit getNextTask, s.o.)
                //- Resette die Variable für die bisher ausgeführten Ticks
    			if(ticks_executed == pow(2,currentQueue)){
    				enqueue(queueArray[currentQueue+1],current);
    				currentQueue = getNextTask(queueArray, num_levels);
    				current = dequeue(queueArray[currentQueue]);
    				ticks_executed = 0;
    			}
    		}
    	}

        //Führe den Current-Task für einen Tick aus
    	execTask(current, 1);

        //Erhöhe ticks_executed
    	ticks_executed++;
    }

    //Lösche alle Queues im Queue-Array und dann das Array selbst
    for(int i=0;i<num_levels;i++){
    	freeQueue(queueArray[i]);
    }
    free(queueArray);
}
