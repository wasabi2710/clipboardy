#include <ws2tcpip.h>
#include <winsock2.h>
#include <stdio.h>
#include "headers/socket_common.h"

#define PORT 1234

struct sockaddr_in server_addr;

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

//client sockfd
void send_data(SOCKET connfd) {
    //prototype: data
    char* buf_data = "im from here";
    recv(connfd, buf_data, strlen(buf_data), 0);
}

//server sockfd
//server requires two socekts
//listening socket & acceptance socket
void server(SOCKET sockfd) {
    SOCKET connfd;
    //client data buffers
    size_t client_buffer_size = BUFSIZ;
    char* client_data = malloc(client_buffer_size);
    //define client(s) properties
    struct sockaddr_in client_addr;
    socklen_t client_addrlen = sizeof(client_addr);
    char client_addr_buffer[INET_ADDRSTRLEN];
    //bind to any interfaces of the device
    //define server properties
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY; // bind to all interfaces
    //bind the socket
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        perror("Server Binding Failed");
        closesocket(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Server binded!\n");
    //listening for connections
    if (listen(sockfd, SOMAXCONN) == SOCKET_ERROR) {
        perror("Server cannot listen for connections");
        closesocket(sockfd);
        exit(EXIT_FAILURE);        
    }
    printf("Server's listening on :1234\n");
    //connections acceptance
    while (1) {
        connfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addrlen);
        if (connfd == SOCKET_ERROR) {
            perror("Client connection failed.");
            continue;
        }
        //convert addr bytes
        inet_ntop(AF_INET, &client_addr.sin_addr, client_addr_buffer, sizeof(client_addr_buffer));
        printf("Connection from: %s:%d\n", client_addr_buffer, htons(client_addr.sin_port));
        //assume: every connection has transfered bytes
        //Receive data from the client
        //memset(client_data, 0, client_buffer_size); // Clear the buffer
        if (recv(connfd, client_data, client_buffer_size, 0) == SOCKET_ERROR) {
            perror("Buffer cannot be received");
            free(client_data);
            //closesocket(connfd);
            continue;
        }
        printf("Message received from client %s:%d {%s}\n", client_addr_buffer, htons(client_addr.sin_port), client_data);

        closesocket(connfd);
    }
}

int main() { 
    SOCKET sockfd;
    sockfd = init_socket();

    // start server
    server(sockfd);

    closesocket(sockfd);
    return 0;
}
