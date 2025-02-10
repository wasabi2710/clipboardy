#include "server.h"
#include "clipboard.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For sleep()

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
        if (currentClipData && (!prevClipData || strcmp(currentClipData, prevClipData) != 0)) {
            relay(sockfd, currentClipData); // Relay the data
            free(prevClipData); // Free previous buffer
            prevClipData = currentClipData;
        } else {
            free(currentClipData); // Free if no change
        }
    }

    // Cleanup
    free(prevClipData);
    close(sockfd); // POSIX uses close() instead of closesocket()
    return 0;
}
