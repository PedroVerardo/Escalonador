#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "Queue.h"

int main(void){
    FILE *fin;
    char process[100];
    char initial_time[10];
    char duration[10];
    int np;

    fin = fopen("/home/pedro/CLionProjects/untitled/exec.txt", "r");
    if(fin == NULL) {
        printf("Path errado");
        exit(-1);
    }

    while(!feof(fin)){

        np = fscanf(fin,"Run %s I=%s D=%s ", process, initial_time, duration);
        pid_t pid = fork();
        sleep(1);
        if(np == 1 && pid == 0){
            char* argument_list[] = {"./round_robin", NULL};
            execvp("./round_robin", argument_list);
        }
        if(np == 3 && pid == 0){
            char* argument_list[] = {"./real_time", initial_time, duration, NULL};
            execvp("./real_time", argument_list);
        }
    }

    fclose(fin);

    return 0;
}