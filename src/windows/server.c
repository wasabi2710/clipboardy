#include "server.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

SOCKET socketCreate() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSA Error: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Winsock initialized\n");

    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == INVALID_SOCKET) {
        printf("WSA Error: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("DG Socket initialized\n");

    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(DEFAULT_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("WSA Error: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("Server binded\n");
    return sockfd;
}

void relay(SOCKET sockfd, const char *clipboard) {
    int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char *)&enable, sizeof(enable)) == SOCKET_ERROR) {
        printf("WSA Error: %d\n", WSAGetLastError());
    }

    struct sockaddr_in broadcastAddr = {0};
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(DEFAULT_PORT);
    
    if (inet_pton(AF_INET, BROADCASTING_ADDR, &broadcastAddr.sin_addr.s_addr) <= 0) {
        printf("Invalid broadcast address\n");
        return;
    }

    if (sendto(sockfd, clipboard, strlen(clipboard), 0, (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr)) == SOCKET_ERROR) {
        printf("WSA Error: %d\n", WSAGetLastError());
    } else {
        printf("Buffer broadcasted\n");
    }
}

void bufferReceiver(SOCKET sockfd, fd_set readfds, struct timeval timeout, char *recBuffer, size_t recBufferSize) {
    struct sockaddr_in senderAddr;
    socklen_t senderAddrLen = sizeof(senderAddr);
    
    timeout.tv_sec = 0;
    timeout.tv_usec = 100000; //100ms

    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);

    int activity = select(sockfd + 1, &readfds, NULL, NULL, &timeout);
    if (activity > 0 && FD_ISSET(sockfd, &readfds)) {
        int receivedData = recvfrom(sockfd, recBuffer, recBufferSize - 1, 0, (struct sockaddr *)&senderAddr, &senderAddrLen);
        if (receivedData == SOCKET_ERROR) {
            printf("WSA Error: %d\n", WSAGetLastError());
            return;
        }

        recBuffer[receivedData] = '\0';
        printf("Message received: %s\n", recBuffer);
    }
}
