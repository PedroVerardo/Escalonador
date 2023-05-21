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
#include <pthread.h>
#include "data_structures.h"

Queue roundRobin;
Node* realTime;

Node* next_realTime;
Process* current_process;

int second_reference;

void runNextRealTime();
void runNextRoundRobin(int second_reference);

void P(char* name) {
    struct timeval start, current;
    printf("[PROCESSO %s] iniciado execucao\n", name);
    gettimeofday(&start, NULL);
    while(1){
        gettimeofday(&current, NULL);
        double elapsed = (current.tv_sec - start.tv_sec) + (current.tv_usec - start.tv_usec) / 1000000.0;
        if (elapsed >= 0.5) {
            printf("[PROCESSO %s] executando\n", name);
            gettimeofday(&start, NULL);
        }
    }
}

// função para saber se o realtime recem escalonado deve ser o próximo da lista
Node* get_closer(int time_reference, Node* p1, Node* p2) {
    
    // o p2 é o recém inserido
    // o p1 é o next e pode ser null
    if (!p1)
        return p2;

    int n1 = p1->process->initial_time;
    int n2 = p2->process->initial_time;
    int t1 = time_reference;
    int t2 = time_reference;

    if (t1 > n1)
        n1 += 60;
    
    if (t2 > n2)
        n2 += 60;

    int diff1 = n1 - t1;
    int diff2 = n2 - t2;

    // printf("diff1 = %d, diff2 = %d\n", diff1, diff2);

    if (diff1 >= diff2)
        return p2;

    return p1;
}



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

void updateRunningProcess(int second_reference) {
    if (!current_process) {
        if (next_realTime) {
            if (next_realTime->process->initial_time == second_reference) {
                // se temos um real time para começar
                runNextRealTime();
                return;
            }
        }
        if (!is_empty(&roundRobin)) {
            runNextRoundRobin(second_reference); 
        }


        return;     
    }

    // printf("oi\n"); 
    if (current_process->end_time == second_reference) {
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

        if (!next_realTime) {
            runNextRoundRobin(second_reference);
            return;
        }

        if (next_realTime->process->initial_time == second_reference) {
            // se temos um real time para começar
            runNextRealTime();
        } else {
            runNextRoundRobin(second_reference);
        }
    }
}

void runNextRealTime() {
    // rodar o next_realtime
    printf("[ESCALONADOR] executando o processo %s (real time)\n", next_realTime->process->name);
    if (kill(next_realTime->process->pid, SIGCONT) == -1) {
        printf("Erro no sinal SIGCONT");
        exit(1);
    }

    // Atulizar o next_realtime e o processo corrente
    current_process = next_realTime->process;
    next_realTime = get_next_process(realTime, next_realTime);
}

void runNextRoundRobin(int second_reference) {
    // rodar proximo da fila roundrobin
    Process* first_roundRobin = dequeue(&roundRobin);
    printf("[ESCALONADOR] executando o processo %s (round robin)\n", first_roundRobin->name);
    if (kill(first_roundRobin->pid, SIGCONT) == -1) {
        printf("Erro no sinal SIGCONT");
        exit(1);
    }

    // o round robin tem um minuto para executar
    first_roundRobin->end_time = (second_reference + 1) % 60;
    current_process = first_roundRobin;
}

void schedule(Process *process_data, pid_t pid, int second_reference) {

    process_data->pid = pid;
    process_data->already_scheduled = 1;

    Process* new_process = createCopy(process_data);

    if (new_process->initial_time == -1) { // round robin
        printf("[ESCALONADOR] alocando %s na fila de prontos round robin\n", process_data->name);
        enqueue(&roundRobin, new_process);

    } else { // real time
        if(!possibleToScheduled(realTime, *process_data)){
            printf("[ESCALONADOR] [TEMPOS INCOMPATIVEIS] nao foi possivel alocar %s na fila de prontos real time\n", process_data->name);
        }
        else{
            printf("[ESCALONADOR] alocando %s na fila de prontos real time\n", process_data->name);
            // precisamos ajustar o próximo real time a ser executado;
            
            Node* inserted = insert_ordered(&realTime, new_process);
            next_realTime = get_closer(second_reference, next_realTime, inserted);
        }
    }
}


void* wait_for_IO(void *IO_process) {
    // Wait for 3 seconds
    sleep(3);
    Process* ready_process = (Process*) IO_process;
    printf("[ESCALONADOR] IO do processo %s finalizado\n", ready_process->name);
    printf("[ESCALONADOR] adicionando o processo %s na fila de prontos\n", ready_process->name);
    enqueue(&roundRobin, ready_process);
    return NULL;
}


void handler(int signum){
    pthread_t thread_id;

    if (pthread_create(&thread_id, NULL, &wait_for_IO, current_process) != 0) {
        printf("Failed to create thread\n");
        return;
    }

    current_process = NULL;
    updateRunningProcess(second_reference);
}


void scheduler(Process* process_data) {
    pid_t pid;
    int scheduler_pid = getpid();
    struct timeval current_time;
    int current_second;

    initialize(&roundRobin);
    realTime = initialize_list();

    signal(SIGUSR1, handler);

    while (1) {

        if (gettimeofday(&current_time, NULL) == -1) {
            printf("Erro na verificação do horário");
            exit(1);
        }
        second_reference = current_time.tv_sec % 60;

        if (current_second != second_reference){
            printf("[CLOCK] segundo atual = %d\n", second_reference);
            current_second = second_reference;
        } 

        // se o programa ainda não tiver sido tratado pelo escalonador
        // criamos um novo processo para executar o programa
        if (!process_data->already_scheduled) {
            pid = fork();
            if (pid == -1) {
                printf("Erro na criação de novo processo");
                exit(1);
            } else if (pid > 0) { // escalonador
                schedule(process_data, pid, second_reference);

                if (!current_process && !next_realTime) {
                    // se for o primeiro processo de todos
                    if (process_data->initial_time == -1) {
                        // caso seja um round robin
                        runNextRoundRobin(second_reference);
                    } else {
                        // caso seja um real time
                        next_realTime = realTime;
                    }
                }
                // printf("bla");
                
            } else { // novo processo
                // o processo coloca a si mesmo em pausa até que o escalonador o escalone
                Process* self_information = createCopy(process_data);

                char buffer_IO[100];
                char buffer_pid[200];

                // Convert the integer to a string
                sprintf(buffer_IO, "%d", self_information->is_IO_bounded);
                sprintf(buffer_pid, "%d", scheduler_pid);

                char *programPath = "./new_process";  // Path to the program you want to execute
                char *args[] = {programPath, self_information->name, buffer_IO, buffer_pid, NULL};  // Arguments for the program, terminated with NULL
                execvp(programPath, args);
            }
                
        }

        updateRunningProcess(second_reference);
    }

}
