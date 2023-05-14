#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include "Queue.h"

Processo* cria_rb(char *processo);
Processo* cria_rt(char *processo, int I, int D);

int main(void){
    FILE *fin;
    int segmento, np, cont, initial_time, duration;
    Processo *var1;
    char process[100], file[100];

    segmento = shmget(7000, sizeof(Processo)*10 ,IPC_CREAT | 0666);
    var1  = (Processo *)shmat(segmento, NULL, 0);

    fin = fopen("./exec.txt", "r");
    if(fin == NULL) {
        printf("Path errado");
        exit(-1);
    }

    while(!feof(fin)) {

        np = fscanf(fin, "Run %s I=%d D=%d\n", file, &initial_time, &duration);
        printf("oi");
        strcpy(process, "./");
        strcat(process, file);
        //sleep(1);
        if(np == 1){
            Processo *b = cria_rb(process);
            var1[cont] = *b;
            cont++;
        }

        if(np == 3){
            //char* argument_list[] = {process, initial_time, duration, NULL};
            //execvp(process, argument_list);
            Processo *b = cria_rt(process, initial_time, duration);
            var1[cont] = *b;
            cont++;
        }
    }

    fclose(fin);
    return 0;
}

Processo* cria_rb(char *processo){
    Processo *novo = (Processo*) malloc(sizeof(Processo));
    strcpy(novo->process_name, processo);
    novo->duracao = 120;
    novo->tempo_ini = 120;

    return novo;
}

Processo* cria_rt(char *processo, int I, int D){
    Processo *novo = (Processo*) malloc(sizeof(Processo));
    strcpy(novo->process_name, processo);
    novo->tempo_ini = I;
    novo->duracao = D;

    return novo;
}