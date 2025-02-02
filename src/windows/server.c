#include "server.h"
#include <psdk_inc/_fd_types.h>
#include <winsock2.h>
#include <ws2tcpip.h>

WSADATA wsaData;
SOCKET sockfd;

struct sockaddr_in serverAddr;

//define handlers for recvfrom
//buffers
size_t recBufferSize = BUFSIZ;
char* recBuffer;
//broadcaster addr
struct sockaddr_in senderAddr;
socklen_t senderAddrLen = sizeof(senderAddr);
char senderAddrIP[INET_ADDRSTRLEN];

//select for recvfrom-blocking thread
fd_set readfds;
struct timeval timeout;

//socket creation
SOCKET socketCreate() {

    // initialize winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("Failed initializing winsock");
        WSACleanup();
        exit(EXIT_FAILURE);        
    }
    printf("Winsock initialzed\n");
    
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == SOCKET_ERROR) {
        perror("Failed initializing datagram socket");
        exit(EXIT_FAILURE);
    }
    printf("DG Socket initialized\n");

    serverAddr.sin_family = AF_INET; //ipv4
    serverAddr.sin_port = htons(DEFAULT_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY; //set up for binding to all interfaces
    
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        perror("Server binding failed");
        closesocket(sockfd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("Server binded\n");

    return sockfd;
}

// clipboard buffer sender
void relay(char* clipboard) {

    //enable broadcasting
    int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char *)&enable, sizeof(enable)) == SOCKET_ERROR) {
        perror("Failed enabling broadcasting");
        exit(EXIT_FAILURE);
    }

    //define broadcasting addr
    struct sockaddr_in broadcastAddr;
    broadcastAddr.sin_family = AF_INET; //ipv4
    broadcastAddr.sin_port = htons(DEFAULT_PORT);
    inet_pton(AF_INET, BROADCASTING_ADDR, &broadcastAddr.sin_addr.s_addr); //convert straddr to binary

    if (sendto(sockfd, clipboard, strlen(clipboard), 0, (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr)) == SOCKET_ERROR) {
        perror("Failed broadcasting buffers");
        exit(EXIT_FAILURE);
    }
    printf("Buffer broadcasted\n");

}

// create listener
void bufferReceiver() {
    // set up select() to wait for either socket or clipboard changes
    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);

    timeout.tv_sec = 0;
    timeout.tv_usec = 100000; //100ms timeout

    int activity = select(sockfd + 1, &readfds, NULL, NULL, &timeout);

    recBuffer = malloc(recBufferSize);

    if (activity > 0 && FD_ISSET(sockfd, &readfds)) {
        int receivedData = recvfrom(sockfd, recBuffer, recBufferSize - 1, 0, (struct sockaddr *)&senderAddr, &senderAddrLen);
        if (receivedData == SOCKET_ERROR) {
            perror("Received Buffer failed");
            free(recBuffer);
        }
        recBuffer[receivedData] = '\0'; //cast null termination
        printf("Message received: %s\n", recBuffer);  
    }
}
