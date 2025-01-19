#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include "src/headers/socket_common.h"

#define PORT 1234
#define HOST "127.0.0.1"

int init_socket() {
    WSADATA wsaData;
    SOCKET sockfd;
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

void client() {
}

int main() {
    //prototype: client
    //open client socket
    struct sockaddr_in server_addr;
    SOCKET sockfd, connfd;
    sockfd = init_socket();
    // client socket does not require bind
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, HOST, &server_addr.sin_addr.s_addr);   
    
    //connect
    connfd = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (connfd == SOCKET_ERROR) {
        perror("Server connection error\n");
        return 1;
    }

    //receive raw bytes
    //use dynamic buffer allocation
    char* buffer;
    if (recv(sockfd, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {
        perror("No message buffer recieved\n");
        free(buffer);
        return 1;
    }

    printf("Message: %s\n", buffer);

    return 0;
}