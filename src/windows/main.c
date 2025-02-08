    #include "server.h"
    #include "clipboard.h"
    #include <winscard.h>
    #include <winsock.h>

    int main() {

        SOCKET sockfd = socketCreate(); // create UDP socket
        fd_set readfds;
        struct timeval timeout;
        size_t recBufferSize = BUFSIZ;
        char* recBuffer = malloc(recBufferSize);
        struct sockaddr_in senderAddr;
        char* prevClipData = NULL;

        while(1) {

            bufferReceiver(sockfd, readfds, timeout,recBuffer, recBufferSize);

            Sleep(100);
            char* currentClipData = clipboard(); //get copied buffers

            if (currentClipData && (!prevClipData || strcmp(currentClipData, prevClipData) != 0)) {
                //printf("Current clipboard: %s\n", currentClipData);
                relay(sockfd, currentClipData); // relay the data
                free(prevClipData); // free prev buffers
                prevClipData = currentClipData;
            } else {
                free(currentClipData);
            }
            
        }
        
        //cleanup
        free(prevClipData);
        closesocket(sockfd);
        WSACleanup();
        return 0;
    }
