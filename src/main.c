#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#endif
#include <stdio.h>

// prototype: socket on windows
void win32Sock() {
    WSADATA wsaData;
    SOCKET sockfd;

    // init winsock
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        printf("Failed to init Winsock. Error code: %d\n", WSAGetLastError());
        return 1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == INVALID_SOCKET) {
        printf("Socket creation failed. Error code: %d\n", WSAGetLastError);
        WSACleanup();
        return 1;
    }

    printf("Socket created successfully\n");

    closesocket(sockfd);
}

// prototype: linux socket.h
void linSock() {

}


int main() {  

    #if defined(_WIN32)
    printf("Socket on Windows\n");
    winsock32();
    #elif
    printf("Socket on Linux\n");
    #endif

    return 0;
}