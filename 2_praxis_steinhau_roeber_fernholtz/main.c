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

    //Wenn das Process-Count-Argument 0 oder weniger ist, setze process_count auf 1
    if(atoi(argv[2])<=0){
    	process_count = 1;
    //Wenn es größer ist als die Länge unseres Alphabets, setze es auf die Länge unseres Alphabets
    }else if(atoi(argv[2])>strlen(charset)){
    	process_count = strlen(charset);
    //Ansonsten setze process_count auf das gegebene Argument
    }else{
    	process_count = atoi(argv[2]);
    }
    min_password_length = atoi(argv[3]);
    max_password_length = atoi(argv[4]);

    //Kopiere das Hash-Argument in den String targetHash
    char* targetHash = (char*) malloc(sizeof(char) * (strlen(argv[1])+1));
    if(targetHash==NULL){
    	perror("Malloc Error");
    	exit(1);
    }
    strncpy(targetHash, argv[1],strlen(argv[1])+1);

    //Alloziiere Speicher für das String-Array work (Für jeden Prozess ein Char-Pointer)
    char** work = (char**) malloc(sizeof(char*) * (process_count+1));
    if(work==NULL){
    	perror("Malloc Error");
    	exit(1);
    }
    //Fülle das String-Array mit den Schranken-Strings mit split_work
    split_work(work, strlen(charset)/process_count);

    //pid wird benutzt um Eltern- und Kindprozesse auseinander zuhalten
    pid_t pid = 0;

    //i wird benutzt um zu zählen, wie viele Kindprozesse bereits erzeugt wurden
    int i = 0;

    //in result wird das richtige Passwort gespeichert, sobald es gefunden wurde
    char* result = (char*) malloc(sizeof(char) * (max_password_length+1));

    time_t t1 = time(NULL);
    //Endlosschleife
    while(1){
    	//Solange i kleiner als process_count ist, forke erneut
        if(i<process_count){
            pid = fork();
        //Ansonsten brich das Forken ab
        }else{
            break;
        }
        //Wenn pid kleiner als 0 ist, ist ein Fehler beim Forken passiert
        if(pid < 0){
            perror("fork");
            exit(1);
        //Wenn pid==0, ist dies der Kindprozess
        }else if(pid == 0){
        	//Im Kindprozess, erhöhe i und gehe an den Anfang der Schleife (forke erneut, außer i=process_count)
            i++;
            continue;
        }
        //Ansonsten ist dies der Elternprozess
        //Rufe brute mit den Schrankenstrings an Stelle i und i+1 auf. Wenn das Ergebnis nicht NULL ist, wurde das Passwort gefunden
        if((result=brute(targetHash,work[i],work[i+1]))!=NULL){
        	//Drucke das gefundene Passwort
            printf("%s\n",result);
            //gib den Speicher für work und targetHash wieder frei
            free_work_data(work,targetHash);
    		time_t t2 = time(NULL) - t1;
    		printf("t2 = %f\n",(double) t2);
    		//Töte alle Prozesse
            kill(0, SIGKILL);
        }
        //Falls brute in diesem Prozess NULL zurückgegeben hat, beende den Prozess (break aus der Schleife -> Programmende)
        break;
    }
   /* CRACK THE FOLLOWING SHA256 HASH:

		fab6b3381ec4dc44084b23206f3a1e7ea3b2fb795a0e5e01e42f6cff11d4c175

	*/
}