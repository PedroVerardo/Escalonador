#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/time.h>
#include "Queue.h"

int main(void){
    int segmento, cont = 0;
    Processo* var;

    Node acctual_node;
    Node *fila_rb = NULL;
    Node *fila_rt = NULL;

    segmento = shmget(7000, sizeof(Processo)*10, IPC_CREAT | 0666);
    var  = (Processo *)shmat(segmento, NULL, 0);

    struct timeval current_time;

    while(1){
        gettimeofday(&current_time, NULL);
        long  sec = current_time.tv_sec%60;
        printf("seconds : %ld", sec);

        //Insere na fila
        sleep(1);
        if(var[cont].duracao == 120)
        {
            fila_rb = insereI(fila_rb, var[cont]);
        }

        else if(var[cont].duracao >= 0 && var[cont].duracao <= 59)
        {
            fila_rt = insereI(fila_rt, var[cont]);
        }
        else{

        }

        //Escalona

        fila_rb = retira(fila_rb, &acctual_node);
        //char* argument_list[] = {acctual_node.val.process_name, NULL};
        //execvp(acctual_node.val.process_name, argument_list);
        fila_rb = insereI(fila_rb, acctual_node.val);



        cont++;
        printf("\nFila Round Robin: ");
        printa(fila_rb);
        printf("\nFila Real Time: ");
        printa(fila_rt);


    }


}
