#include <network.h>
#include <server.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <signal.h>

void sigchld_handler(int s) {
  // waitpid() might overwrite errno, so we save and restore it;
  int saved_errno = errno;

  while (waitpid(-1, NULL, WNOHANG) > 0);

  errno = saved_errno;
}

void start_server(const int port){
  printf("starting server at port %d ...\n", port);

  int sockfd, new_fd;     // listen on sock_fd, new connection on new_fd
  struct addrinfo hints, *serverinfo, *p;
  struct sockaddr_storage their_addr; // connector's address information 

  socklen_t sin_size;
  struct sigaction sa;
  int yes = 1;
  char s[INET6_ADDRSTRLEN];
  int rv;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;  // use my IP  
  
  if ((rv = getaddrinfo(NULL, port_as_str(port), &hints, &serverinfo )) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return;
  }
 
  //loop through all the results and bind to the first we can 
  for (p = serverinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) { 
      perror("setsockopt");
      exit(1);
    }
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("server: bind");
      continue;
    }
    break;
  }

  freeaddrinfo(serverinfo);
  if (p == NULL) {
   fprintf(stderr, "server: failed to bin\n"); 
   exit(1);
  }
  if (listen(sockfd, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }
  sa.sa_handler = sigchld_handler; //reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  printf("Server set.\nWaiting for connections...\n");
  while(1) { //main accept loop
    sin_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr* )&their_addr, &sin_size);
    if (new_fd == -1) {
      perror("accept");
      continue;
    }
    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr* )&their_addr), s, sizeof s);
    printf("server: got connection from %s\n", s);
    if (!fork()) { //this is the child process
      close(sockfd); //child does not need the listener
      if (send(new_fd, "Hello, world!", 13, 0) == -1) {
        perror("send");
      }
      close(new_fd);
      exit(0);
    }
    close(new_fd);  //parent doesn't need this
  }
}
