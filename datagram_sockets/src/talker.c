#include <talker.h>
#include <network.h>

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>

void start_talker(const char* hostname, const int port, const char* message)
{
  printf("starting talker: %s:%d - %s...\n", hostname, port, message);

  int sockfd;
  struct addrinfo hints, *serverinfo, *p;
  int rv;
  int numbytes;
  
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET6;
  hints.ai_socktype = SOCK_DGRAM;

  if ((rv = getaddrinfo(hostname, port_as_str(port), &hints, &serverinfo)) == -1) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return;
  }
  // loop through all the results and make a socket

  for (p = serverinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("talker: socket");
      continue;
    }
    break;
  }

  if (p == NULL) {
    fprintf(stderr, "talker: failed to create socket\n");
    return;
  }
  if ((numbytes = sendto(sockfd, message, strlen(message), 0, p->ai_addr, p->ai_addrlen)) == -1) {
    perror("talker: sendto");
    exit(1);
  }
  
  freeaddrinfo(serverinfo);
  printf("talker: sent %d bytes to %s\n", numbytes, hostname);
  
}
