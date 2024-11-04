#include <network.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>


// get sockaddr, IPv4 or IPv6
void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in* )sa)->sin_addr);
  }
  return &(((struct sockaddr_in6* )sa)->sin6_addr);
}

const char* port_as_str(const int port) {
  char* port_str = (char*)malloc(6*sizeof(char));
  sprintf(port_str, "%d", port);
  return (const char*)port_str;
}

