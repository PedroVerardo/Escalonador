//
// Created by pedro on 12/05/23.
//

#ifndef UNTITLED_QUEUE_H
#define UNTITLED_QUEUE_H

struct node {
    int val;
    struct node *prox;
};
typedef struct node Node;

Node *insereI(Node *lista, int val);

void printa(Node *lista);

Node *retira_elem(Node *lista, int val);

Node *retira(Node *lista);

int isempty(Node *lista);

#endif //UNTITLED_QUEUE_H
