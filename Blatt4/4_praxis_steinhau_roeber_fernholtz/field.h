#include <pthread.h>
#include <stdbool.h>

#include "config.h"

typedef struct animal{
	int type;
	int age;
	int energy;
	int last_modified;
}animal;

typedef struct field{

	//TODO: declare mutex and other variables for synchronization of threads

	pthread_mutex_t field_printed_mutex;
	pthread_cond_t field_printed_cond;
	bool printed;

	pthread_mutex_t* lower_row_mutex;
	pthread_mutex_t* upper_row_mutex;

	pthread_cond_t* lower_row_cond;
	pthread_cond_t* upper_row_cond;

	bool* lower_row_beeing_read;
	bool* upper_row_beeing_read;

	animal** mtx;

	int num_threads;
	int width;
	int height;
	int generation;

}field;

field* make_field(int num_threads);