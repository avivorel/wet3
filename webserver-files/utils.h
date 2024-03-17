//
// Created by Aviv on 03/03/2024.
//
#ifndef WET3_UTILS_H
#define WET3_UTILS_H






#include <sys/time.h>

typedef struct Queue *Queue;
typedef struct Node *Node;

Node createNode(int value, struct timeval arrival);
Queue createQueue(int size);
int queueCurrentSize(Queue queue);
int isFull(Queue queue);
int isQueueEmpty(Queue queue);
void enqueue(Queue queue, int value, struct timeval arrival);
struct timeval headArrivalTime(Queue queue);
int dequeue(Queue queue);
int findInQueue(Queue queue, int value);
int dequeueByIndex(Queue queue, int index);
void freeQueue(Queue queue);


#endif //WET3_UTILS_H
