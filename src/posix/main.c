#include "server.h"
#include "clipboard.h"

int main() {
    int sockfd = socketCreate(); // create UDP socket
    fd_set readfds;
    struct timeval timeout;
    size_t recBufferSize = 65507;
    char* recBuffer = malloc(recBufferSize);
    char* prevClipData = NULL;

    while (1) {
        bufferReceiver(sockfd, &readfds, &timeout, recBuffer, recBufferSize);
        usleep(100000); // Sleep for 100ms (POSIX compatible)

        char* currentClipData = clipboard(); // Get copied buffers
        if (currentClipData) {
            printf("Current Clipboard Data: %s\n", currentClipData); // Debug print
            if (!prevClipData || strcmp(currentClipData, prevClipData) != 0) {
                printf("Clipboard has changed. Relay data...\n");
                relay(sockfd, currentClipData); // Relay the data
                free(prevClipData); // Free previous buffer
                prevClipData = currentClipData;
            } else {
                free(currentClipData); // Free if no change
            }
        }
    }

    // Cleanup
    free(prevClipData);
    close(sockfd); // POSIX uses close() instead of closesocket()
    return 0;
}
