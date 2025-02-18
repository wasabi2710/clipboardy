#include "server.h"
#include "clipboard.h"

int socketCreate() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("DG Socket initialized\n");

    int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable)) < 0) {
        perror("setsockopt failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(DEFAULT_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server binded\n");
    return sockfd;
}

void relay(int sockfd, const char *clipboard) {
    if (!clipboard || *clipboard == '\0') return; // Don't send empty messages

    struct sockaddr_in broadcastAddr = {0};
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(DEFAULT_PORT);

    if (inet_pton(AF_INET, BROADCASTING_ADDR, &broadcastAddr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid broadcast address\n");
        return;
    }

    if (sendto(sockfd, clipboard, strlen(clipboard), 0, (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr)) < 0) {
        perror("Send failed");
    } else {
        printf("Buffer broadcasted\n");
    }
}

void bufferReceiver(int sockfd, fd_set *readfds, struct timeval *timeout, char *recBuffer, size_t recBufferSize) {
    struct sockaddr_in senderAddr;
    socklen_t senderAddrLen = sizeof(senderAddr);

    FD_ZERO(readfds);
    FD_SET(sockfd, readfds);

    int activity = select(sockfd + 1, readfds, NULL, NULL, timeout);
    if (activity > 0 && FD_ISSET(sockfd, readfds)) {
        int receivedData = recvfrom(sockfd, recBuffer, recBufferSize - 1, 0, (struct sockaddr *)&senderAddr, &senderAddrLen);
        if (receivedData <= 0) {
            fprintf(stderr, "Receive failed or empty message\n");
            return;
        }

        recBuffer[receivedData] = '\0';
        write_clipboard(recBuffer);
    }
}
