#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>
#include "data_structures.h"


int main(int argc, char *argv[]){
    struct timeval start, current;
    char* name = argv[1];

    printf("\n\n%d\n\n", (int)argv[2][0]);
    int shm_pid_id = shmget(5000, sizeof(pid_t), IPC_CREAT | 0666);
    pid_t *pai = (pid_t*) shmat(shm_pid_id, NULL, 0);

    if (kill(getpid(), SIGSTOP) == -1) {
        printf("Erro no sinal SIGSTOP");
        exit(1);
    }

    printf("[PROCESSO %s] iniciado execucao\n", name);
    gettimeofday(&start, NULL);
    while(1){
        gettimeofday(&current, NULL);
        double delta = (current.tv_sec - start.tv_sec) + (current.tv_usec - start.tv_usec) / 1000000.0;
        if (delta >= 0.5) {
            printf("[PROCESSO %s] executando\n", name);
            gettimeofday(&start, NULL);
        }

        if((int)argv[2][0] == 49){
            kill(pai[0], SIGUSR1);
        }
    }
}