#include "server.h"
#include "clipboard.h"
#include <winsock.h>

int main() {

    SOCKET sockfd = socketCreate(); // create UDP socket
    
    char* prevClipData = NULL;
    while(1) {

        bufferReceiver();

        Sleep(100);
        char* currentClipData = clipboard(); //get copied buffers

        if (currentClipData && (!prevClipData || strcmp(currentClipData, prevClipData) != 0)) {
            //printf("Current clipboard: %s\n", currentClipData);
            relay(currentClipData); // relay the data
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
