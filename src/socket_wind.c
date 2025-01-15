#include <ws2tcpip.h>
#include <winsock2.h>
#include <stdio.h>
#include "headers/socket_common.h"

int init_socket() {
    WSADATA wsaData;
    SOCKET sockfd;

    // init winsock
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        printf("Failed to init Winsock. Error code: %d\n", WSAGetLastError());
        return 1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == INVALID_SOCKET) {
        printf("Socket creation failed. Error code: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    printf("Socket created successfully\n");

    return sockfd;
}

int main() { 
    SOCKET sockfd, connfd; 
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    char clientIp[INET_ADDRSTRLEN];

    printf("Socket on Windows\n");
    sockfd = init_socket();

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(1234);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    
    // bind socket
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        perror("Failed bind");
        closesocket(sockfd);    
        exit(EXIT_FAILURE);
    }

    printf("Binded successfully!\n");

    // listen for connection
    if (listen(sockfd, 5) == SOCKET_ERROR) {
        perror("Failed listening");
        closesocket(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on :1234 ...\n");

    // acceptance
    while (1) {

        connfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (connfd == SOCKET_ERROR) {
            perror("Failed acceptitng connection");
            continue;
        }

        //connection str conversion
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, INET_ADDRSTRLEN);
        printf("Connection from %s:%d\n", clientIp, ntohs(clientAddr.sin_port));

        closesocket(connfd);
    }

    closesocket(sockfd);

    return 0;
}
