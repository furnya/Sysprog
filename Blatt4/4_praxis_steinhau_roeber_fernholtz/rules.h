#include <stdint.h>
#include <stdlib.h>

enum field_type {FISH, SHARK, WATER};

// secure memory allocation
void* emalloc(size_t size);

void get_area(void* t_args, int* start, int* end);

void calculate_shark_generation(void* args);

void calculate_fish_generation(void* args);