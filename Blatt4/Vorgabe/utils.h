
//secure memory allocation
void* emalloc(size_t size);

//get random walk direction from h and w
void get_walk_dir(int h, int w, int* n_h, int* n_w);

// get this thread's area of responsibility
void get_area(void* t_args, int* start, int* end);