#ifndef SERVER_H
#define SERVER_H

#include <bits/types/struct_timeval.h>
#define DEFAULT_PORT 1234
#define BROADCASTING_ADDR "127.0.0.1"

#define SOCKET_ERROR -1

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>

int socketCreate(); 
void relay(int sockfd, char *clipboard);
void bufferReceiver(int sockfd, fd_set readfds, struct timeval timeout, char* recBuffer, size_t bufferSize, struct sockaddr_in *senderAddr);

#endif