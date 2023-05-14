#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structures.h"

Node* initialize_list() {
    return NULL;
}

void insert_ordered(Node **head, Process *new_process) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) {
        perror("malloc");
        exit(1);
    }
    new_node->process = new_process;
    new_node->next = NULL;

    if (*head == NULL || (*head)->process->initial_time > new_process->initial_time) {
        new_node->next = *head;
        *head = new_node;
    } else {
        Node *current = *head;
        while (current->next != NULL && current->next->process->initial_time < new_process->initial_time) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

void print_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("Process name: %s, initial_time: %d\n", current->process->name, current->process->initial_time);
        current = current->next;
    }
}

Node* get_next_process(Node* head, Node* element) {
    if (element->next == NULL) {
        return head;
    }
    return element->next;
}

// int main() {
//     Node *head = NULL;

//     // Creating processes
//     Process *process1 = (Process *)malloc(sizeof(Process));
//     strcpy(process1->name, "Process 1");
//     process1->initial_time = 20;

//     Process *process2 = (Process *)malloc(sizeof(Process));
//     strcpy(process2->name, "Process 2");
//     process2->initial_time = 10;

//     Process *process3 = (Process *)malloc(sizeof(Process));
//     strcpy(process3->name, "Process 3");
//     process3->initial_time = 30;

//     // Insert processes into the ordered list
//     insert_ordered(&head, process1);
//     insert_ordered(&head, process2);
//     insert_ordered(&head, process3);

//     // Print the ordered list
//     print_list(head);

//     return 0;
// }
