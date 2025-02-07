#include "server.h"
#include <sys/select.h>
#include <sys/socket.h>

int sockfd;

int socketCreate() {
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == SOCKET_ERROR) {
        perror("Failed creating socket");
        exit(EXIT_FAILURE);
    }
    printf("sockfd created\n");
    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET; //ipv4
    serverAddr.sin_port = htons(DEFAULT_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if(bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        perror("Faile: bind socket address");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Server binded\n");
    return sockfd;
}

void relay(int sockfd, char* clipboard) {
    int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable)) == SOCKET_ERROR) {
        perror("Failed: enabling broadcasting");
        free(clipboard);
    }
    struct sockaddr_in broadcastAddr = {0};
    broadcastAddr.sin_family = AF_INET; //ipv4
    broadcastAddr.sin_port = htons(DEFAULT_PORT);
    if (inet_pton(AF_INET, BROADCASTING_ADDR, &broadcastAddr.sin_addr.s_addr) != 1) {
        perror("Failed: invalid broadcasting address");
        free(clipboard);
    }    
    if (sendto(sockfd, clipboard, sizeof(clipboard), 0, (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr)) == SOCKET_ERROR ) {
        perror("Failed: broadcasting data");
        free(clipboard);
    }
    printf("Buffer broadcasted\n");
}

void bufferReceiver(int sockfd, fd_set readfds, struct timeval timeout, char* recBuffer, size_t bufferSize, struct sockaddr_in *senderAddr) {
    socklen_t senderAddrLen = sizeof(*senderAddr);
    FD_ZERO(&readfds); //ensure readfds context is empty
    FD_SET(sockfd, &readfds);
    timeout.tv_sec = 0;
    timeout.tv_usec = 100000; //100ms
    int activity = select(sockfd + 1, &readfds, NULL, NULL, &timeout);
    if (activity > 0 && FD_ISSET(sockfd, &readfds)) { //sockfd mvmnt detection
        int receivedData = recvfrom(sockfd, recBuffer, bufferSize - 1, 0, (struct sockaddr *)senderAddr, &senderAddrLen);
        if (receivedData == SOCKET_ERROR) {
            perror("Received buffer failed");
            free(recBuffer);
            return;
        }
        recBuffer[receivedData] = '\0';
        printf("Message received: %s\n", recBuffer);
    }   
}