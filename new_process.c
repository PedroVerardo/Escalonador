#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[]){
    struct timeval start, current;
    char* name = argv[1];

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
    }
}