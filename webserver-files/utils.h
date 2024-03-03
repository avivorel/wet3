//
// Created by Aviv on 03/03/2024.
//
#ifndef WET3_UTILS_H
#define WET3_UTILS_H

#include <stdbool.h>





#include <stdbool.h>
#include <sys/time.h>

typedef struct Queue *Queue;
typedef struct Node *Node;

Node createNode(int value, struct timeval arrival);
Queue createQueue(int size);
int queueCurrentSize(Queue queue);
bool isFull(Queue queue);
bool isQueueEmpty(Queue queue);
void enqueue(Queue queue, int value, struct timeval arrival);
struct timeval queue_head_arrival_time(Queue queue);
int dequeue(Queue queue);
int queue_find(Queue queue, int value);
int dequeue_index(Queue queue, int index);
void freeQueue(Queue queue);
void queue_print(Queue queue);


#endif //WET3_UTILS_H
