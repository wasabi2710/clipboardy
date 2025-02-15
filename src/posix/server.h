#ifndef SERVER_H
#define SERVER_H

#define DEFAULT_PORT 1234
#define BROADCASTING_ADDR "192.168.1.255"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // For close()
#include "clipboard.h"

// Function declarations for POSIX server
int socketCreate();
void bufferReceiver(int sockfd, fd_set *readfds, struct timeval *timeout, char* recBuffer, size_t recBufferSize);
void relay(int sockfd, const char* clipboard);

#endif
