#include "server.h"
#include "clipboard.h"

int main() {
    SOCKET sockfd = socketCreate(); // create UDP socket
    fd_set readfds;
    struct timeval timeout;
    size_t recBufferSize = 65507;
    char* recBuffer = malloc(recBufferSize);
    struct sockaddr_in senderAddr;
    char* prevClipData = NULL;

    if (!recBuffer) {
        perror("Memory allocation failed");
        return EXIT_FAILURE;
    }

    while (1) {
        bufferReceiver(sockfd, readfds, timeout, recBuffer, recBufferSize);
        Sleep(1000);

        char* currentClipData = clipboard(); // get copied buffers
        if (currentClipData && (!prevClipData || strcmp(currentClipData, prevClipData) != 0)) {
            // printf("Current clipboard: %s\n", currentClipData);   
            relay(sockfd, currentClipData); // relay the data
            free(prevClipData); // free previous buffers
            prevClipData = currentClipData;
        } else {
            free(currentClipData);
        }
    }

    // Cleanup
    free(recBuffer);
    free(prevClipData);
    closesocket(sockfd);
    WSACleanup();
    return 0;
}
