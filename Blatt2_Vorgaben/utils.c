#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "utils.h"
#include "bruteforce.h"


void split_work(char** work, int step)
{
    for(int i=0; i < process_count+1; i++)
    {   
        // Construct smallest/lowest possible
        if(i == 0)
        {
            work[i] = malloc(sizeof(char) * min_password_length+1);

            for(int min=0; min < min_password_length; min++)
                work[i][min] = charset[0];

            work[i][min_password_length] = '\0';

            continue;
        }

        // Construct biggest/highest possible
        if(i == process_count)
        {
            work[i] = malloc(sizeof(char) * max_password_length+1);

            for(int max=0; max < max_password_length; max++)
                work[i][max] = charset[strlen(charset)-1];

            work[i][max_password_length] = '\0';

            break;
        }

        // Step up
        work[i] = malloc(sizeof(char) * max_password_length+1);
        
        for(int k=0; k < max_password_length; k++)
            work[i][k] = charset[step*i];
        
        work[i][max_password_length] = '\0';
    }
}


void free_work_data(char** work, char* target)
{
    for(int i=0; i < process_count+1; i++)
    {
        free(work[i]);
        work[i] = NULL;
    }
    
    free(work);
    free(target);
    work = NULL;
    target = NULL;
}


int* gen_indices(const char* word, int len)
{
    int charset_length = strlen(charset);
    int* indices = malloc(sizeof(int) * len);

    for(int i=0; i < len; i++)
        for(int k=0; k < charset_length; k++)
            if(charset[k] == word[i])
                indices[i] = k;

    return indices;
}


int* expand_indices(int* indices, int base)
{
    indices = realloc(indices, sizeof(int) * base);

    for(int i=0; i<base; i++)
        indices[i] = 0;

    return indices;
}


char* expand_buffer(char* buffer, int base)
{
    buffer = realloc(buffer, sizeof(char) * (base+1));

    for(int i=0; i<base; i++)
        buffer[i] = charset[0];

    buffer[base] = '\0';

    return buffer;
}