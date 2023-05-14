#include <stdio.h>
#include <stdlib.h>
#include "data_structures.h"

void initialize(Queue *queue) {
    queue->front = 0;
    queue->rear = -1;
}

int is_empty(Queue *queue) {
    return queue->front > queue->rear;
}

int is_full(Queue *queue) {
    return queue->rear == MAX_PROCESS_COUNT - 1;
}

void enqueue(Queue *queue, Process* value) {
    if (is_full(queue)) {
        printf("Error: Queue is full\n");
        exit(1);
    }
    queue->arr[++queue->rear] = value;
}

Process* dequeue(Queue *queue) {
    if (is_empty(queue)) {
        printf("Error: Queue is empty\n");
        exit(1);
    }
    return queue->arr[queue->front++];
}

void printQueue(Queue *queue) {
    if (is_empty(queue)) {
        printf("Queue is empty\n");
        return;
    }

    printf("Queue elements:\n");

    for (int i = queue->front; i <= queue->rear; i++) {
        printf("Element %d: %s\n", i, queue->arr[i]->name);
    }
}


// int main() {
//     Queue my_queue;
//     initialize(&my_queue);

//     Process p1 = {"teste", 0, 0, 0};
//     Process p2 = {"teste2", 0, 0, 0};
//     Process p3 = {"teste3", 0, 0, 0};

//     // Enqueue elements
//     enqueue(&my_queue, &p1);
//     enqueue(&my_queue, &p2);
//     enqueue(&my_queue, &p3);

//     // Dequeue elements and print their values
//     printf("Dequeued value: %s\n", dequeue(&my_queue)->name);
//     printf("Dequeued value: %s\n", dequeue(&my_queue)->name);
//     printf("Dequeued value: %s\n", dequeue(&my_queue)->name);

//     return 0;
// }
