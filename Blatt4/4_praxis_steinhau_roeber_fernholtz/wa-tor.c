#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "field.h"
#include "rules.h"
#include "thread.h"
#include "printer.h"
#include "utils.h"

int main(){

	// declare threads
	pthread_t threads[NUM_THREADS], printer;

	// create game field
	field* field = make_field(NUM_THREADS);

	// check if number of threads is accurate
	if(NUM_THREADS < 1){
		perror("There must be at least one thread!");
		exit(1);
	} else if(NUM_THREADS > field -> height){
		perror("Too many threads!");
		exit(1);
	}

	//start threads
	for (int i = 0; i<NUM_THREADS; i++){
		thread_args *t_args = (thread_args*) emalloc(sizeof(thread_args));
		t_args->id = (unsigned int) i;
		t_args->field = field;

		if(0 != pthread_create(&threads[i], NULL, thread, (void*) t_args)){
			perror("Error while creating the threads");
			exit(1);
		}
	}
	//start printer
	if(0 != pthread_create(&printer, NULL, print_game, (void*) field)){
		perror("Error while creating the printer thread");
		exit(1);
	}

	//join threads
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}
	//join printer
	pthread_join(printer, NULL);

	return 0;
}
