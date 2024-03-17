#include <stdio.h>
#include "utils.h"
#include <stdlib.h>



struct Queue {
    Node head;
    Node tail;
    int currentSize;
    int maxSize;
};

struct Node {
    int data;
    struct timeval arrival;
    Node next;
};


Node createNode(int value, struct timeval arrival){
    Node node = (Node)malloc(sizeof(*node));
    node->data = value;
    node->arrival = arrival;
    node->next = NULL;
    return node;
}

Queue createQueue(int size){
    Queue queue = (Queue)malloc(sizeof(*queue));
    queue->head = NULL;
    queue->tail = NULL;
    queue->currentSize = 0;
    queue->maxSize = size;
    return queue;
}


void enqueue(Queue queue, int value, struct timeval arrival){
    if(isFull(queue))
        return;

    Node node = createNode(value, arrival);
    if(isQueueEmpty(queue)){
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }

    queue->currentSize++;
}

struct timeval headArrivalTime(Queue queue){
    if(isQueueEmpty(queue))
        return (struct timeval){0};
    return queue->head->arrival;
}

int dequeue(Queue queue){
    if(isQueueEmpty(queue))
        return -1;
    Node temp = queue->head->next;
    int value = queue->head->data;
    free(queue->head);
    if(temp == NULL){
        queue->head = NULL;
        queue->tail = NULL;
    } else {
        queue->head = temp;
    }
    queue->currentSize--;
    return value;
}

int findInQueue(Queue queue, int value){
    if(isQueueEmpty(queue))
        return -1;
    Node temp = queue->head;
    int index = 0;
    while(temp){
        if(value == temp->data){
            return index;
        }
        index++;
        temp = temp->next;
    }

    return -1;
}

int dequeueByIndex(Queue queue, int index){
    if(isQueueEmpty(queue))
        return -1;

    if(index < 0 || index >= queue_size(queue))
        return -1;

    if(index == 0){
        return dequeue(queue);
    }

    Node toRemove = queue->head;
    Node previous = NULL;
    for(int i = 0; i < index; i++){
        previous = toRemove;
        toRemove = toRemove->next;
    }

    int value = toRemove->data;
    previous->next = toRemove->next;
    free(toRemove);
    if(index == queue_size(queue) - 1){
        queue->tail = previous;
    }
    queue->currentSize--;

    return value;
}

int queueCurrentSize(Queue queue){
    return queue->current_size;
}

void freeQueue(Queue queue){
    Node current = queue->head;
    Node next = NULL;
    while (current){
        next = current->next;
        free(current);
        current = next;
    }
    free(queue);
}


int isFull(Queue queue){
    if( queue->currentSize == queue->maxSize)
        return 1;
    return 0
}

int isQueueEmpty(Queue queue){
    if( queue->currentSize == 0)
        return 1;
    return 0;
}
