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
#include <showip.h>



void start_showip_example(int argc, char *argv[]) {
  if (argc != 2) {
    printf("[showip error] usage: showip hostname\n");
    return;
  }

  struct addrinfo hints, *res, *p;
  int status;
  char ipstr[INET6_ADDRSTRLEN];
  
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC; //AF_INET Or AF_INET6 to force version
  hints.ai_socktype = SOCK_STREAM;

  if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return;
  }
  printf("IP addresses for %s:\n\n", argv[1]);

  for (p = res; p != NULL; p = p->ai_next) {
    void* addr;
    char* ipver;
    
    // get the pointer to the address itself,
    // different fields in IPv4 and IPv6:
    
    if (p->ai_family == AF_INET) { //IPv4
      struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
      addr = &(ipv4->sin_addr);
      ipver = "IPv4";
    } else {  //IPv6
      struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)p->ai_addr;
      addr = &(ipv6->sin6_addr);
      ipver = "IPv6";
    }
    //Convert string to int and print it
    inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    printf("\t%s: %s\n", ipver, ipstr);
  }

  freeaddrinfo(res);
}
