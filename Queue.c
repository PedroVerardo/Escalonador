#include <stdio.h>
#include <stdlib.h>

struct node {
    int val;
    struct node *prox;
};
typedef struct node Node;


Node *insereI(Node *lista, int val) {
    Node *novo = (Node *)malloc(sizeof(Node));

    novo->val = val;
    novo->prox = lista;

    lista = novo;

    return lista;
}


void printa(Node *lista) {
    printf("\n");
    for (Node *p = lista; p != NULL; p = p->prox) {
        printf("%d ", p->val);
    }
    printf("\n");
}


Node *retira_elem(Node *lista, int val) {
    Node *ant = NULL;
    Node *p = lista;

    while (p != NULL && p->val != val) {
        ant = p;
        p = p->prox;
    }

    if (p == NULL) {return lista;}

    if (ant == NULL) {lista = p->prox;}

    else {ant->prox = p->prox;}

    free(p);
    return lista;
}


Node *retira(Node *lista){
    Node *ant = NULL;
    Node *p = lista;

    if(lista == NULL){
        return NULL;
    }

    while (p != NULL) {
        ant = p;
        p = p->prox;
        if(p->prox == NULL){
            ant->prox = NULL;
            free(p);
            return lista;
        }
    }
}


int isempty(Node *lista){

    if(lista == NULL){return 0;}

    return 1;
}