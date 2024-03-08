#ifndef __REQUEST_H__

void requestHandle(int fd, struct timeval arrival, struct timeval handleTime, int *staticThreads, int *dynamicThreads,
                   int *totalThreads, int threadIndex);

#endif
