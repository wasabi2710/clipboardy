#include <string.h>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <stdio.h>
#include "headers/socket_common.h"

WSADATA wsa_data;
SOCKET dgsock;

// prototype: datagram socket
int main() {

    //windows has to enable winsock first
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        perror("WSAStartup failed.");
        exit(EXIT_FAILURE);        
    }
    
    //establish datagram socket
    dgsock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (dgsock == SOCKET_ERROR) {
        perror("Socket creation failed");
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("Socket established\n");

    //bind the socket
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET; //ipv4
    server_addr.sin_port = htons(CLIPPORT);
    server_addr.sin_addr.s_addr = ADDR_ANY;

    if (bind(dgsock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        perror("Error binding server");
        exit(EXIT_FAILURE);
    }
    printf("Server binded\n");

    // clients    
    char server_ip[INET_ADDRSTRLEN]; // buffer for IP address
    char server_info[INET_ADDRSTRLEN + 10]; // buffer to store IP + port
    struct sockaddr_in client_addr;
    socklen_t clientaddr_len = sizeof(client_addr);
    char addr_len[INET_ADDRSTRLEN];
    size_t buffer_size = BUFSIZ;
    char* incoming_buffer = malloc(buffer_size);

    //convert server IP to string
    inet_ntop(AF_INET, &server_addr.sin_addr, server_ip, sizeof(server_ip));

    // Format the IP and port into a string
    snprintf(server_info, sizeof(server_info), "%s:%d", server_ip, ntohs(server_addr.sin_port));

    //accept the connections and get the data from the server
    while(ACTIVE) {
        int bytes_received = recvfrom(dgsock, incoming_buffer, buffer_size - 1, 0, (struct sockaddr *)&client_addr, &clientaddr_len);
        if (bytes_received == SOCKET_ERROR) {
            perror("Error reading buffer from broadcast");
            continue;
        }   
        incoming_buffer[bytes_received] = '\0'; // null termination

        inet_ntop(AF_INET, &client_addr.sin_addr, addr_len, sizeof(addr_len));
        printf("Received from: %s:%d with message: %s\n", addr_len, htons(client_addr.sin_port), incoming_buffer);

        if (strcmp(incoming_buffer, "clip") == 0) {
            //send the server IP and port string back to the client
            if (sendto(dgsock, server_info, strlen(server_info), 0, (struct sockaddr *)&client_addr, sizeof(client_addr)) == SOCKET_ERROR) {
                perror("Message sent failed!");
                continue;
            }
            printf("Server IP and port sent: %s\n", server_info);
        } else {
            printf("No suitable servers\n");
        }
    }

    //cleanups
    free(incoming_buffer);
    closesocket(dgsock);

    return 0;
}
