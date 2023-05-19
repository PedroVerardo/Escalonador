#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "process_structure.h"
#include "interpreter.h"

Process* getProcessData(char* process_name, int initial_time, int duration) {
    Process* process = (Process*) malloc(sizeof(Process));
    strcpy(process->name, process_name);
    process->initial_time = initial_time;
    if (initial_time == -1) {
        process->end_time = duration;
    } else {
        process->end_time = initial_time + duration;
    }
    process->already_scheduled = 0;
    process->is_running = 0;
    process->is_ready = 1;
    return process;
}

void interpreter(Process* process_data) {

    int number_parameters, initial_time, duration;
    char program_name[MAX_NAME_LEN];
    Process* current_process;

    //  abrindo o arquivo de processos
    FILE *process_file = fopen("exec.txt", "r");
    if (!process_file) {
            printf("Error: fopen failed\n");
            exit(1);
        }

    // percorremos o arquivo linha por linha até o final
    while (!feof(process_file)) {

        number_parameters = fscanf(process_file, "Run %s I=%d D=%d\n", program_name, &initial_time, &duration);

        sleep(1);
        if (number_parameters == 1) {
            // processo round robin
            printf("[INTERPRETADOR] O processo %s (round robin) foi recebido\n", program_name);
            current_process = getProcessData(program_name, -1, -1);
        } else if (number_parameters == 3) {
            // processo real time
            printf("[INTERPRETADOR] O processo %s (real time com I=%d e D=%d) foi recebido\n", program_name, initial_time, duration);
            current_process = getProcessData(program_name, initial_time, duration);
        } else {
            printf("Erro na leitura do arquivo");
            exit(1);
        }

        // guardando as informações do processo lido na memória compartilhada
        *process_data = *current_process; 
        sleep(1);
        
    }

    fclose(process_file);

}