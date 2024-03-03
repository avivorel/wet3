#include "segel.h"
#include "request.h"
#include "utils.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

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
pthread_cond_t blockCond;
Queue requestQueue = NULL;
Queue workingThreadsQueue = NULL;


// Linked list
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


void* threadFunction(void* threadArgs){

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
    listenfd = Open_listenfd(port);
    while (1) {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
	requestHandle(connfd);
	Close(connfd);
    }

}


    


 
