#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "SDL.h"

#include "config.h"
#include "field.h"
#include "rules.h"
#include "thread.h"
#include "utils.h"

void* thread(void* t_args){
	thread_args* args = (thread_args*) t_args;
	field* field = args -> field;

	// find out area of responsipility
	int start, end;
	get_area(args, &start, &end);

	while(1){

		// make sure that field has already been printed before modifying it
		pthread_mutex_lock(&field -> field_printed_mutex);
			while(!field -> printed){
				pthread_cond_wait(&field -> field_printed_cond, &field-> field_printed_mutex);
			}
		pthread_mutex_unlock(&field -> field_printed_mutex);

		// update field
		calculate_shark_generation((void*)args); 
		calculate_fish_generation((void*) args);

		//TODO: make sure all threads finished updating their areas before proceeding
		//TODO: increase generation number of field struct

		// signalize printer to print new generation
		pthread_mutex_lock(&field -> field_printed_mutex);
			field -> printed = false;
			pthread_cond_signal(&field -> field_printed_cond);
		pthread_mutex_unlock(&field -> field_printed_mutex);
	}


	return 0;
}
