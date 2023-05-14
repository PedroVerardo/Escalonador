#include "process_structure.h"

typedef struct Node {
    Process *process;
    struct Node *next;
} Node;

Node* initialize_list();
void insert_ordered(Node **head, Process *new_process);
void print_list(Node *head);
Process* get_next_process(Node* head, Node* element);
