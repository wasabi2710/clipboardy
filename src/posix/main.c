#include "server.h"
#include "clipboard.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    int sockfd = socketCreate(); // create UDP socket
    fd_set readfds;
    struct timeval timeout;
    size_t recBufferSize = 65507;
    char* recBuffer = malloc(recBufferSize);
    char* prevClipData = NULL;
    char* currentClipData = NULL;

    if (!recBuffer) {
        perror("Failed to allocate receive buffer");
        return 1;
    }

    while (1) {
        bufferReceiver(sockfd, &readfds, &timeout, recBuffer, recBufferSize);
        usleep(100000); // Sleep for 100ms

        currentClipData = clipboard(); // Get copied buffers
        
        if (currentClipData && (!prevClipData || strcmp(currentClipData, prevClipData) != 0)) {
            printf("Clipboard has changed. Relaying data...\n");
            relay(sockfd, currentClipData); // Relay the data
            free(prevClipData);
            prevClipData = strdup(currentClipData); // Properly duplicate the string
        }
        free(currentClipData); // Free if no change
    }

    // Cleanup (this part is never reached in an infinite loop, consider handling termination signals)
    free(recBuffer);
    free(prevClipData);
    close(sockfd);
    return 0;
}
