#define _POSIX_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <math.h>

#include "main.h"
#include "utils.h"
#include "bruteforce.h"


int main(int argc, char* argv[])
{
    // Process Commandline arguments
    if(argc != 5 || !strcmp("-h", argv[1]) || !strcmp("--help", argv[1]))
    {
        printf("\nUsage: %s <sha256 hash> <process count> <min> <max>\n\n", argv[0]);
        printf("\t\t<sha256 hash>  \t = 64 character long hex character sequence\n");
        printf("\t\t<process count>\t = Number of processes to split workload to\n");
        printf("\t\t<min>          \t = lowest number of cleartext length\n");
        printf("\t\t<max>          \t = highest number of cleartext length\n\n");
        return 0;
    }

    process_count = atoi(argv[2]);
    min_password_length = atoi(argv[3]);
    max_password_length = atoi(argv[4]);

	/* CRACK THE FOLLOWING SHA256 HASH:

		fab6b3381ec4dc44084b23206f3a1e7ea3b2fb795a0e5e01e42f6cff11d4c175

	*/

    char* targetHash = (char*) malloc(sizeof(char) * (strlen(argv[1])+1));
    strncpy(targetHash, argv[1],strlen(argv[1])+1);

    char** work = (char**) malloc(sizeof(char*) * (process_count+1));
    if(strlen(charset)/process_count>0){
      split_work(work, strlen(charset)/process_count);
    }else{
      split_work(work, strlen(charset));
    }
    pid_t pid1 = 0;
    int i = 0;
    char* result = (char*) malloc(sizeof(char) * (max_password_length+1));
    time_t t1 = time(NULL);
    int processes = strlen(charset)<process_count ? strlen(charset) : process_count;
    // printf("t1 = %f\n",(double) t1);
    while(i>=0){
        if(i<processes){
            pid1 = fork();
            // printf("i=%d | %u\n",i,pid1);
        }else{
            break;
        }
        if(pid1 < 0){
            perror("fork");
            exit(1);
        }else if(pid1 == 0){
            i++;
            continue;
        }
        if((result=brute(targetHash,work[i],work[i+1]))!=NULL){
            printf("%s\n",result);
            free_work_data(work,targetHash);
    		time_t t2 = time(NULL) - t1;
    		printf("t2 = %f\n",(double) t2);
            kill(0, SIGKILL);
        }
        break;
    }
}
