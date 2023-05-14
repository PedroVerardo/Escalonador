#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include "data_structures.h"

#define SHM_KEY 1235

int main() {

    // Obtendo a área de memória compartilhada
    int shm_id = shmget(SHM_KEY, sizeof(Process), IPC_CREAT | 0666);
    if (shm_id == -1) {
        printf("Error: shmget failed bla bls\n");
        exit(1);
    }

    // obtendo o endereço da memória compartilhada
    Process *process_data = (Process *)shmat(shm_id, NULL, 0);
    if (process_data == (Process *)-1) {
        printf("Error: shmat failed\n");
        exit(1);
    }

    // Vamos criar um novo processo para executar o escalonador e o interpretador de forma concorrente
    pid_t pid = fork();

    if (pid == -1) {
        printf("Error: fork failed");
    } else if (pid == 0) { // escalonador
        scheduler(process_data);
    } else { // Interpretador
        interpreter(process_data);
    }

    if (shmdt(process_data) == -1) {
        printf("Error: shmdt failed\n");
        exit(1);
    }

    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        printf("Error: shmctl failed\n");
        exit(1);
    }
    
    return 0;
}

