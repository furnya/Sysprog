#include "FCFS.h"
#include "TaskQueue.h"
#include <stdio.h>
#include <stdlib.h>

void schedule_FCFS(const TaskPool *task_pool) {
    //Falls der Task-Pool leer ist, oder die Länge aller Prozesse nicht mindestens 1 ist, tue nichts
    if(task_pool->size <= 0 || task_pool->total_duration <= 0){
    	return;
    }

    //Erstelle eine Queue, in der Tasks gespeichert werden können
    Queue* queue = (Queue*) malloc(sizeof(Queue));
    if(queue==NULL){
    	perror("Malloc Error");
    	exit(1);
    }
    queue->size = 0;

    //Tick speichert das Zeitintervall, in dem wir uns befinden
    int tick = 0;

    //Current ist der Task, der momentan ausgeführt wird
    Task* current;

    //arrived wird verwendet, um einen Task, der im momentanen Tick ankommt (arrive), zwischenzuspeichern, bevor er in die Queue eingereiht wird
    Task* arrived;

    //done wird verwendet, um zu speichern, ob der momentane Prozess fertig ist (am Anfang muss dies true sein, damit der erste Prozess ausgeführt wird)
    bool done = true;

    //Bis alle Tasks fertig sind, führe die Schleife aus
    while(!allDone(task_pool) && tick < task_pool->total_duration){

        //Prüfe, ob in diesem Tick ein Task ankommt. Speicher diesen in arrived
    	arrived = checkArrivals(task_pool,tick);

        //Falls ein Task angekommen ist (!=NULL), reihe ihn in die Queue ein
    	if(arrived != NULL){
    		enqueue(queue,arrived);
    	}

        //Falls im letzten Tick der ausgeführte Task beendet wurde (done = true), hole einen neuen Task aus der Queue und setze done wieder auf false
    	if(done){
    		current = dequeue(queue);
    		done = false;
    	}

        //Führe den current-Task für einen Tick aus
    	execTask(current,1);

        //Falls der ausgeführte Task fertig ist, setze done auf true
    	if(isDone(current)){
    		done = true;
    	}

        //Erhöhe den Tick
    	tick++;
    }

    //Lösche die Queue
    freeQueue(queue);
}
