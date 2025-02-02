#include "headers/socket_common.h"
#include <complex.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define PORT 1234

int init_socket() {
  int sockfd;
  sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sockfd < 0) {
    perror("Unix SysSocket cannot be established");
    return 1;
  }
  printf("Socket established: %d\n", sockfd);

  return sockfd;
}

int main() {
  int sockfd, confd; 
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_len = sizeof(client_addr);
  printf("Building on Linux");
  sockfd = init_socket(); 
  char client_ip[INET_ADDRSTRLEN];

  memset(&server_addr, 0, sizeof(server_addr)); // zero the structure
  server_addr.sin_family = AF_INET; // ipv4
  server_addr.sin_addr.s_addr = INADDR_ANY; // bind to all available interfaces
  server_addr.sin_port = htons(PORT); // bind to port 1234

  if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    printf("Cannot bind addr\n");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  printf("Bind Successfull!\n");
  
  if (listen(sockfd, 5) < 0) {
    printf("Listen failed!\n");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  printf("Ready for connections\n");
  printf("Server is listening on port 1234...\n");

  while(1) {
    confd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
    if (confd < 0) {
      perror("Accept failed\n"); 
      continue;
    }

    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    printf("Connected from %s:%d\n", client_ip, ntohs(client_addr.sin_port));

    close(confd);
  }

  close(sockfd);
  return 0;
}
