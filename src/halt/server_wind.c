#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/ssl.h>

#include "headers/socket_common.h"

int main() {
    // Define datagram socket
    WSADATA wsaData;
    SOCKET dgsock;

    // Enable Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("WSAStartup failed");
        exit(EXIT_FAILURE);
    }

    // Create UDP socket
    dgsock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (dgsock == INVALID_SOCKET) {
        perror("UDP Socket Error");
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("Socket established!\n");

    // Enable broadcasting
    int enable = 1; // Enable broadcasting (set to 1)
    if (setsockopt(dgsock, SOL_SOCKET, SO_BROADCAST, (char*)&enable, sizeof(enable)) == SOCKET_ERROR) {
        perror("Failed setting socket option");
        closesocket(dgsock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Configure the broadcast address
    struct sockaddr_in broad_addr;
    broad_addr.sin_family = AF_INET;
    broad_addr.sin_port = htons(CLIPPORT); // Use the defined port number
    inet_pton(AF_INET, BROADCAST_ADDR, &broad_addr.sin_addr.s_addr); // Convert the broadcast address to binary form

    // Send data to the broadcast address
    const char* buf_data = "clip";
    if (sendto(dgsock, buf_data, strlen(buf_data), 0, (struct sockaddr*)&broad_addr, sizeof(broad_addr)) == SOCKET_ERROR) {
        perror("Error sending data to broadcast address");
        closesocket(dgsock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("Message broadcasted!\n");

    //listen for clients
    struct sockaddr_in client_addr;
    size_t msg_buffer_size = BUFSIZ;
    socklen_t clientaddr_len = sizeof(client_addr);
    char* incoming_buffer = malloc(msg_buffer_size);
    while(ACTIVE) {

        int bytes_received = recvfrom(dgsock, incoming_buffer, msg_buffer_size - 1, 0, (struct sockaddr *)&client_addr, &clientaddr_len);
        if (bytes_received == SOCKET_ERROR) {
            perror("Failed receiving message");
            continue;
        }
        incoming_buffer[bytes_received] = '\0'; // null terminator

        printf("Incoming message: %s\n", incoming_buffer);

    }

    // Cleanup
    closesocket(dgsock);
    WSACleanup();

    return 0;
}
