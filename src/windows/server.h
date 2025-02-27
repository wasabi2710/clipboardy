#ifndef SERVER_H
#define SERVER_H

#define DEFAULT_PORT 1234
#define BROADCASTING_ADDR "192.168.1.255"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include "clipboard.h"

SOCKET socketCreate();
void bufferReceiver(SOCKET sockfd, fd_set readfds, struct timeval timeout, char* recBuffer, size_t recBufferSize);
void relay(SOCKET sockfd, const char* clipboard);

#endif