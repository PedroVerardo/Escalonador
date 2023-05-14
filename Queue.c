#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"


Node *insereI(Node *lista, Processo val) {
    Node *novo = (Node *)malloc(sizeof(Node));

    novo->val = val;
    novo->prox = lista;

    lista = novo;

    return lista;
}


void printa(Node *lista) {
    for (Node *p = lista; p != NULL; p = p->prox) {
        printf("%s ",p->val.process_name);
    }
    printf("\n");
}


Node *retira(Node *lista, Node *removed){
    Node *ant = NULL;
    Node *b = NULL;
    Node *p = lista;
    Node acctual;

    if(lista == NULL)
    {
        return lista;
    }

    while (p->prox != NULL) {
        ant = p;
        p = p->prox;
    }

    if(ant == NULL)
    {
        lista = p->prox;
    }

    else
    {
        ant->prox = NULL;
    }

    *removed = *p;
    free(p);


    return lista;
}


int isempty(Node *lista){

    if(lista == NULL){return 0;}

    return 1;
}