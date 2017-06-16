typedef struct thread_args{
    int id;
    field* field;
}thread_args;

// function as parameter for new thread created
void* thread(void* t_args);
