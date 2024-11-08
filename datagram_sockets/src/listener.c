#include <listener.h>
#include <network.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

void start_listener(const int port)
{

  printf("starting listener: %d...\n", port);

  int sockfd;
  struct addrinfo hints, *serverinfo, *p;
  int rv;
  int numbytes;
  struct sockaddr_storage their_addr;
  char buf[MAXBUFLEN];
  socklen_t addr_len;
  char s[INET6_ADDRSTRLEN];

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET6; // set to AF_INET to use IPv4
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE; // use my IP
  if ((rv = getaddrinfo(NULL, port_as_str(port), &hints, &serverinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return;
  }

  // loop through all the results and bind to the first one we can
  for (p=serverinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("listener: socket");
      continue;
    }
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("listener: bind");
      continue;
    }
    break;
  }

  if (p == NULL) {
    fprintf(stderr, "listener: failed to bind socket\n");
    return;
  }
  freeaddrinfo(serverinfo);

  printf("listener: waiting to recvfrom...\n");
  addr_len = sizeof their_addr;

  if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1, 0, (struct sockaddr*)&their_addr, &addr_len)) == -1) {
    perror("recvfrom");
    exit(1);
  }
  printf("listener: got packet from %s\n",
     inet_ntop(their_addr.ss_family,
       get_in_addr((struct sockaddr*)&their_addr),
      s, sizeof s));
  printf("listener: packet is %d bytes long\n", numbytes);
  buf[numbytes] = '\0';
  printf("listener: packet contains \"%s\"\n", buf);
  close(sockfd);
}
