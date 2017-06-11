#include "RR.h"
#include "TaskQueue.h"
#include <stdio.h>
#include <stdlib.h>

void schedule_RR(const TaskPool *task_pool, uint16_t quantum_max) {
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

    //Current ist der Task, der momentan ausgeführt wird
    Task* current = NULL;

    //Tick speichert das Zeitintervall, in dem wir uns befinden
    int tick = 0;

    //arrived wird verwendet, um einen Task, der im momentanen Tick ankommt (arrive), zwischenzuspeichern, bevor er in die Queue eingereiht wird
    Task* arrived;

    //Bis alle Tasks fertig sind, führe die Schleife aus
    while(!allDone(task_pool)){

        //Prüfe, ob in diesem Tick ein Task ankommt. Speicher diesen in arrived
    	arrived = checkArrivals(task_pool,tick);

        //Falls ein Task angekommen ist (!=NULL), reihe ihn in die Queue ein
        if(arrived != NULL){
    		enqueue(queue, arrived);
    	}

        //Falls der im letzten Round-Robin-Intervall (der letzte Durchlauf der While-Schleife) ausgeführte Task nicht fertig ist, reihe ihn wieder in die Queue ein
    	if(!isDone(current)){
    		enqueue(queue, current);
    	}

        //Nimm den Kopf aus der Queue und setze ihn als neuen current-Task
    	current = dequeue(queue);

        //Führe die innere Schleife quantum_max mal aus (außer der momentane Task ist vorher fertig ausgeführt)
    	for(int i=0;i<quantum_max && !isDone(current);i++){

            //Prüfe in jedem Durchlauf, ob ein neuer Task ankommt (außer im ersten Durchlauf bei i=0, da wurde es vor der inneren Schleife schon geprüft) und reihe ihn ggf. in die Queue ein
    		if(i!=0){
    			arrived = checkArrivals(task_pool,tick);
        		if(arrived != NULL){
    				enqueue(queue, arrived);
    			}
    		}

            //Führe den current-Task aus und erhöhe den Tick
    		tick += execTask(current, 1);
    	}
    }

    //Lösche die Queue
    freeQueue(queue);
}
