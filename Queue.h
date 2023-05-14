//Estruturas

struct processo{
    char process_name[100];
    int tempo_ini;
    int duracao;
};
typedef struct processo Processo;

struct node {
    Processo val;
    struct node *prox;
};
typedef struct node Node;


//Funcoes

Node *insereI(Node *lista, Processo val); //insere no inicio

void printa(Node *lista); //printa elementos da fila

Node *retira(Node *lista, Node *removed); //retira o ultimo elemento da fila

int isempty(Node *lista); //devolve se a fila está vazia
