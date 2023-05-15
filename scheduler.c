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

Queue roundRobin;
Node* realTime;

Node* next_realTime;
Process* current_process;

//this function return true if the process can be sheduled.
int possibleToScheduled(Node* head, Process real_time_process){
    if(head == NULL){
        return 1;
    }
    Node* current = head;
    int time_ini;
    int time_end;

    while(current != NULL){
        time_ini = current->process->initial_time;
        time_end = current->process->end_time;

        if(real_time_process.initial_time >= time_ini
            && real_time_process.initial_time < time_end){
            return 0;
        }

        if(real_time_process.end_time >= time_ini
            && real_time_process.end_time < time_end){
            return 0;
        }

        if(real_time_process.initial_time < time_ini
            && real_time_process.end_time > time_end){
            return 0;
        }
        current = current->next;
    }
    return 1;
}

Process* createCopy(Process* original) {
    // Allocate memory for the new struct
    Process* copy = (Process*)malloc(sizeof(Process));

    // Copy the data from the original struct to the new copy
    if (copy != NULL) {
        strcpy(copy->name, original->name);
        copy->initial_time = original->initial_time;
        copy->end_time = original->end_time;
        copy->is_running = original->is_running;
        copy->is_ready = original->is_ready;
        copy->is_IO_bounded = original->is_IO_bounded;
        copy->already_scheduled = original->already_scheduled;
        copy->pid = original->pid;
    }

    return copy;
}

void updateRunningProcess(int minute_reference) {

    if (current_process->end_time == minute_reference) {
        // precisamos escolher o novo processo a ser executado

        // stop o current process
        if (kill(current_process->pid, SIGSTOP) == -1) {
            printf("Erro no sinal SIGSTOP");
            exit(1);
        }

        if (current_process->initial_time == -1) {
            // se for um round robin, volta para o fim da fila
            enqueue(&roundRobin, current_process);
        }


        if (next_realTime->process->initial_time == minute_reference) {
            // se temos um real time para começar
            // rodar o next_realtime
            if (kill(next_realTime->process->pid, SIGCONT) == -1) {
                printf("Erro no sinal SIGCONT");
                exit(1);
            }
            printf("[ESCALONADOR] executando o processo %s (real time)", next_realTime->process->name);

            // Atulizar o next_realtime e o processo corrente
            current_process = next_realTime->process;
            next_realTime = get_next_process(realTime, next_realTime);

        } else {

            // rodar proximo da fila roundrobin
            Process* first_roundRobin = dequeue(&roundRobin);
            if (kill(first_roundRobin->pid, SIGCONT) == -1) {
                printf("Erro no sinal SIGCONT");
                exit(1);
            }
            printf("[ESCALONADOR] executando o processo %s (round robin)", first_roundRobin->name);

            // o round robin tem um minuto para executar
            first_roundRobin->end_time = (minute_reference + 1) % 60;
            current_process = first_roundRobin;
        }
    }
}

void schedule(Process *process_data, pid_t pid) {

    process_data->pid = pid;
    process_data->already_scheduled = 1;

    Process* new_process = createCopy(process_data);

    if (new_process->initial_time == -1) { // round robin
        printf("[ESCALONADOR] alocando %s na fila de prontos round robin\n", process_data->name);
        enqueue(&roundRobin, new_process);

    } else { // real time
        if(!possibleToScheduled(realTime, *process_data)){
            printf("[ESCALONADOR] nao foi possivel alocar %s na fila de prontos real time\n", process_data->name);
        }
        else{
            printf("[ESCALONADOR] alocando %s na fila de prontos real time\n", process_data->name);
            insert_ordered(&realTime, new_process);
        }
    }
}

void scheduler(Process* process_data) {
    pid_t pid;
    struct timeval current_time;
    int minute_reference;

    initialize(&roundRobin);
    realTime = initialize_list();

    while (1) {
        // se o programa ainda não tiver sido tratado pelo escalonador
        // criamos um novo processo para executar o programa
        if (!process_data->already_scheduled) {
            pid = fork();
            if (pid == -1) {
                printf("Erro na criação de novo processo");
                exit(1);
            } else if (pid > 0) { // escalonador

                if (kill(pid, SIGSTOP) == -1) {
                    printf("Erro no sinal SIGSTOP");
                    exit(1);
                }
                schedule(process_data, pid);

                if (!current_process) {
                    // primeiro processo de todos
                    // continue;
                    // printf("oioi\n");
                }
                
            } else { // novo processo

                char* program = process_data->name;
                char* arguments[] = {program, NULL};
                char* executable = "./";
                strcat(executable, program);
                execvp(executable, arguments);
                exit(1);
            }
                
        }

        if (gettimeofday(&current_time, NULL) == -1) {
            printf("Erro na verificação do horário");
            exit(1);
        }

        minute_reference = current_time.tv_sec % 60;  
        // updateRunningProcess(minute_reference);
        
    }

}
