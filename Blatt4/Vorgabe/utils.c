#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "field.h"
#include "thread.h"

//secure memory allocation
void* emalloc(size_t size) {
	void *buf = malloc(size);
	if(buf == NULL){
		perror("memory allocation failed");
		exit(1);
	}
	return buf;
}

//get random walk direction from h and w
void get_walk_dir(int h, int w, int* n_h, int* n_w){
	int dir = random()%8;

	if(dir < 3){
		*n_h = h-1;
	} else if(dir < 5){
		*n_h = h;
	} else {
		*n_h = h+1;
	}

	if(dir == 0 || dir == 3 || dir == 5){
		*n_w = w-1;
	} else if(dir == 1 || dir == 6){
		*n_w = w;
	} else {
		*n_w = w+1;
	}	
}

// get this thread's area of responsibility
void get_area(void* t_args, int* start, int* end){
	thread_args* args = (thread_args*) t_args;
	int id = args -> id;
	field* field = args -> field;

	int area_size = field -> height / field -> num_threads;
	
	*start = id * area_size;
	if(id == field -> num_threads-1){
		*end = field -> height - 1;
	} else {
		*end = *start + area_size - 1;
	}
}