#include "segel.h"
#include "request.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "utils.h"

// 
// server.c: A very, very simple web server
//
// To run:
//  ./server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//


pthread_mutex_t m;
pthread_cond_t cond;
pthread_cond_t condWait;
pthread_cond_t blockCond;
Queue requestQueue = NULL;
Queue workingThreadsQueue = NULL;


int* staticThreads;
int* dynamicThreads;
int* totalThreads;
// Queue


// HW3: Parse the new arguments too
void getargs(int *port, int argc, char *argv[])
{
    if (argc < 2) {
	fprintf(stderr, "Usage: %s <port>\n", argv[0]);
	exit(1);
    }
    *port = atoi(argv[1]);
}


_Noreturn void* threadFunction(void* threadArgs){
    int threadIndex = ((int*)threadArgs)[0];

    while(1){
        pthread_mutex_lock(&m);
        //lock -> check if queue empty -> wait if so -> continue
        while(isQueueEmpty(requestQueue)){
            pthread_cond_wait(&cond,&m);
        }
        struct timeval arrival = headArrivalTime(requestQueue);
        int fd = dequeue(requestQueue);
        enqueue(workingThreadsQueue,fd,arrival);
        pthread_mutex_unlock(&m);


        struct timeval handle;
        gettimeofday(&handle, NULL);
        // TODO: we need to calc handle-arrival and return it somewhere
        requestHandle(fd, arrival, handle, staticThreads, dynamicThreads, totalThreads, threadIndex);
        Close(fd);

        pthread_mutex_lock(&m);
        dequeueByIndex(workingThreadsQueue,findInQueue(workingThreadsQueue,fd));
        pthread_cond_signal(&condWait);
        pthread_mutex_unlock(&m);
    }
}

int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;
    getargs(&port, argc, argv);
    int threadCount = atoi(argv[1]);
    int requestsCount = atoi(argv[2]);
    workingThreadsQueue = createQueue(threadCount);
    requestQueue = createQueue(requestsCount);
    pthread_t* threads = malloc(sizeof(*threads)*threadCount);
    for(int i = 0; i< threadCount; i++){
        int threadArgs[] = {i,};
        pthread_create(&threads[i], NULL, threadFunction, (void *)threadArgs);
    }
    dynamicThreads = malloc(sizeof(int)*threadCount);
    staticThreads = malloc(sizeof(int)*threadCount);
    totalThreads = malloc(sizeof(int)*threadCount);
    for(int i =0; i < threadCount; i++){
        dynamicThreads[i] = 0;
        staticThreads[i] = 0;
    }

    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_cond_init(&condWait, NULL);

    listenfd = Open_listenfd(port);
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
        pthread_mutex_lock(&m);
        if(queueCurrentSize(workingThreadsQueue) + queueCurrentSize(requestQueue) == requestsCount){
            if(strcmp(argv[4], "block") == 0){
                while(queueCurrentSize(workingThreadsQueue) + queueCurrentSize(requestQueue) == requestsCount){
                    pthread_cond_wait(&blockCond, &m);
                }
            } else if(strcmp(argv[4], "dh") == 0){
                if(isQueueEmpty(requestQueue)){
                    Close(connfd);
                    pthread_mutex_unlock(&m);
                    continue;
                }
                else {
                    int fd = dequeue(requestQueue);
                    Close(fd);
                }
            } else if(strcmp(argv[4], "random") == 0){
                if(isQueueEmpty(requestQueue)){
                    Close(connfd);
                    pthread_mutex_unlock(&m);
                    continue;
                }
                else {
                    int num_to_drop = (int)((queueCurrentSize(requestQueue) + 1) / 2);
                    for(int i = 0; i < num_to_drop; i++) {
                        if(isQueueEmpty(requestQueue))
                            break;
                        int random_index = rand() % queueCurrentSize(requestQueue);
                        int fd = dequeueByIndex(requestQueue, random_index);
                        Close(fd);
                    }
                }
            } else if(strcmp(argv[4], "dt") == 0){
                Close(connfd);
                pthread_mutex_unlock(&m);
                continue;
            }
        }
        struct timeval arrival;
        gettimeofday(&arrival,NULL);

        enqueue(requestQueue, connfd, arrival);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&m);
    }
}


    


 
