#include "server.h"
#include "clipboard.h"
#include <signal.h>

int sockfd;
char* recBuffer = NULL;
char* prevClipData = NULL;

void cleanup(int sig) {
    free(recBuffer);
    free(prevClipData);
    close(sockfd);
    exit(0);
}

int main() {
    sockfd = socketCreate(); // create UDP socket
    fd_set readfds;
    struct timeval timeout;
    size_t recBufferSize = 65507;
    recBuffer = malloc(recBufferSize);
    
    if (!recBuffer) {
        perror("Failed to allocate receive buffer");
        return 1;
    }

    signal(SIGINT, cleanup);
    signal(SIGTERM, cleanup);

    while (1) {
        bufferReceiver(sockfd, &readfds, &timeout, recBuffer, recBufferSize);

        char* currentClipData = clipboard();
        if (currentClipData && (!prevClipData || strcmp(currentClipData, prevClipData) != 0)) {
            printf("Clipboard has changed. Relaying data...\n");
            relay(sockfd, currentClipData);
            free(prevClipData);
            prevClipData = currentClipData; // Avoid unnecessary strdup
        } else {
            free(currentClipData);
        }
    }

    return 0;
}
