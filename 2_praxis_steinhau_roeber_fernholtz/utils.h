#ifndef UTILS_H
#define UTILS_H

void    split_work      (char**, int);
void    free_work_data  (char**, char*);
int*    gen_indices     (const char*, int);
int*    expand_indices  (int*, int);
char*   expand_buffer   (char*, int);

#endif