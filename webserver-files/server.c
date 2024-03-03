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


void print_m(void* arg){

}


int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;
    getargs(&port, argc, argv);
    struct Queue* requestQueue = createLinkedList();
    int threadCount = atoi(argv[1]);
    pthread_t* threads = (pthread_t*) malloc(threadCount * sizeof(pthread_t));

    for(int i = 0; i < threadCount ; i++){
        pthread_t t;
        pthread_create(&t, NULL, print_m, NULL);
    }


    listenfd = Open_listenfd(port);
    while (1) {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);

	// 
	// HW3: In general, don't handle the request in the main thread.
	// Save the relevant info in a buffer and have one of the worker threads 
	// do the work. 
	// 
	requestHandle(connfd);

	Close(connfd);
    }

}


    


 
