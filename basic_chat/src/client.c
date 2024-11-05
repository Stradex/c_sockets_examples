#include <network.h>
#include <client.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

int get_listener_socket_client(const char* hostname, int port) {
  int sockfd, numbytes;
  struct addrinfo hints, *serverinfo, *p;
  int rv;
  char s[INET6_ADDRSTRLEN];

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((rv = getaddrinfo(hostname, port_as_str(port), &hints, &serverinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    exit(1);
  }

  // loop through all the results and connect to the first we can
  for (p=serverinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1 ) {
      perror("client: socket");
      continue;
    }
    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("client: connect");
      continue;
    }
    break;
  }

  if (p == NULL) {
    fprintf(stderr, "client: failed to connect\n");
    return -1;
  }
  inet_ntop(p->ai_family, get_in_addr((struct sockaddr*)p->ai_addr), s, sizeof s);
  printf("client: connected to %s\n", s);
  freeaddrinfo(serverinfo);

  return sockfd;
}

void start_client(const char* hostname, const char* username, int port)
{
  printf("[client] connecting to %s:%d...\n", hostname, port);


  char buf[MAXDATASIZE];
  int sockfd, nbytes;

  sockfd = get_listener_socket_client(hostname, port);
  if (sockfd == -1) {
    fprintf(stderr, "client: error getting socket\n");
    exit(1);
  }

  struct pollfd* pfds = malloc(2*sizeof(*pfds));
  pfds[POLL_STDIN].fd = 0; //STDIN
  pfds[POLL_STDIN].events = POLLIN; // report ready-to-read events
  pfds[POLL_SERVER].fd = sockfd; //SOCKET
  pfds[POLL_SERVER].events = POLLIN; // report ready-to-read events

  for (;;) {
    int poll_count = poll(pfds, MAX_POLLS, -1);
    if (poll_count == -1) {
      perror("poll");
      exit(1);
    }
    for (int i=0; i < MAX_POLLS; i++) {
      if (pfds[i].revents & POLLIN) {
        memset(buf, 0, sizeof(buf));
        switch (i){
          case POLL_STDIN:
            char input_buf[MAXINPUTSIZE];
            nbytes = read(0, input_buf, MAXINPUTSIZE-1);
            input_buf[nbytes-1] = '\0';
            sprintf(buf, "%s: %s", username, input_buf);
            if (send(sockfd, buf, strlen(buf), 0) == -1) { 
              printf("client: failed to send message\n");
            }
          break;
          case POLL_SERVER:
            nbytes = recv(pfds[i].fd, buf, sizeof buf, 0);
            if (nbytes <= 0) {
              if (nbytes == 0) {
                fprintf(stderr, "client: server stopped\n");
              } else {
                fprintf(stderr, "client: recv error\n");
              }

              exit(1);
            }

            printf("%s\n", buf);
          break;

        }
      }
    }    
  }

  close(sockfd);
}
