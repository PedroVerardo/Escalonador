#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"


Node *insereILinked(Node *lista, Processo val) {
    Node *novo = (Node *)malloc(sizeof(Node));
    novo->val = val;
    novo->prox = lista;
    lista = novo;
    return lista;
}

void printaLinked(Node *lista) {
    for (Node *p = lista; p != NULL; p = p->prox) {
        printf("%d ", p->val.process_name);
    }
}

Node *retiraLinked(Node *lista, Processo val,int ini) {
    Node *ant = NULL;
    Node *p = lista;
    while (p != NULL && p->val.tempo_ini != ini) {
        ant = p;
        p = p->prox;
    }
    if (p == NULL) {
        return lista;
    }
    if (ant == NULL) {
        lista = p->prox;
    } else {
        ant->prox = p->prox;
    }
    free(p);
    return lista;
}

Node *insereOrdenado(Node *lista, Node* novo, int a) {
    Node *ant = NULL;
    Node *p = lista;

    while (p != NULL && p->val.tempo_ini < a)
    {
        ant = p;
        p = p->prox;
    }

    if (ant == NULL)
    {
        novo->prox = lista;
        lista = novo;
    }

    else
    {
        novo->prox = ant->prox;
        ant->prox = novo;
    }

    return lista;
}
