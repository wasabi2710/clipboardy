#include "headers/socket_common.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>

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
  int sockfd;
  const struct sockaddr_in addr;

  printf("Building on Linux");
  sockfd = init_socket(); 
  //bind to localhost
  bind(sockfd, )

  return 0;
}
