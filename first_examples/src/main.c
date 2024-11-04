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

//Where I should continue reading: Chapter 5: socket()—Get the File Descriptor!

const char* stringify_sockaddr6(struct sockaddr_in6* sa6) {
  char* buffer = (char*)malloc((INET6_ADDRSTRLEN + 16)*sizeof(char));
  char ip6[INET6_ADDRSTRLEN];
  inet_ntop(AF_INET6, &(sa6->sin6_addr), ip6, INET6_ADDRSTRLEN);
  sprintf(buffer, "IPv6 address: %s", ip6);
  return (const char*)buffer;
}


const char* stringify_sockaddr(struct sockaddr_in* sa) {
  char* buffer = (char*)malloc((INET_ADDRSTRLEN + 16)*sizeof(char));
  char ip4[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(sa->sin_addr), ip4, INET_ADDRSTRLEN);
  sprintf(buffer, "IPv4 address: %s", ip4);
  return (const char*)buffer;
}

void test_sockaddr() {
  struct sockaddr_in sa; //IPv4
  struct sockaddr_in6 sa6; //IPv6

  //IPs strings to address structs.  
  inet_pton(AF_INET, "10.12.110.57", &(sa.sin_addr));
  inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &(sa6.sin6_addr));
  
  printf("IPv4 DATA:\n\tsin_family: %d\n\tsin_port: %d\n\tsin_addr: %d\n", sa.sin_family, sa.sin_port, (int)sa.sin_addr.s_addr);
  printf("IPv6 DATA:\n\tsin6_family: %d\n\tsin6_port: %d\n\tsin6_addr: %d\n", sa6.sin6_family, sa6.sin6_port, (int)sa6.sin6_addr.s6_addr);

  //Address structs to IPs strings
  char ip4[INET_ADDRSTRLEN];
  char ip6[INET6_ADDRSTRLEN];
  inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);
  inet_ntop(AF_INET6, &(sa6.sin6_addr), ip6, INET6_ADDRSTRLEN);

  printf("\nIPv4 address: %s\nIPv6 address: %s\n",ip4, ip6); 
}

const char* stringify_serverinfo(struct addrinfo* serverinfo) {
  char* buffer = NULL;
  // serverinfo now points to a linked list of 1 or more struct addrinfo
  for (int i=1; serverinfo; serverinfo=serverinfo->ai_next, i++) {
    
    int buffer_element_size = INET6_ADDRSTRLEN + 150; 
    char* old_buffer = NULL; 
    if (i > 1) {
      old_buffer = (char*)malloc((i-1)*buffer_element_size*sizeof(char));
      strcpy(old_buffer, buffer);
    }

    buffer = (char*)realloc(buffer, i*buffer_element_size*sizeof(char));
    const char* ai_family_str = serverinfo->ai_family == AF_INET ? "AF_INET" : (serverinfo->ai_family == AF_INET6 ? "AF_INET6" : "AF_UNSPEC" ); 
    const char* ai_socktype_str = serverinfo->ai_socktype == SOCK_STREAM ? "SOCK_STREAM" : "SOCK_DGRAM";
    const char* ai_protocol_str = serverinfo->ai_protocol == IPPROTO_TCP ? "IPPROTO_TCP" : (serverinfo->ai_protocol == IPPROTO_UDP ? "IPPROTO_UDP" : "Other"); 
    const char* socket_addr_str = serverinfo->ai_family == AF_INET6 ? stringify_sockaddr((struct sockaddr_in*)serverinfo->ai_addr) : stringify_sockaddr6((struct sockaddr_in6*)serverinfo->ai_addr);

    if (old_buffer) {
      sprintf(buffer, "%sserverinfo\n\tai_family: %s\n\tai_socktype: %s\n\tai_protocol: %s\n\t%s\n\n",old_buffer, ai_family_str, ai_socktype_str, ai_protocol_str, socket_addr_str);
    } else {
      sprintf(buffer, "serverinfo\n\tai_family: %s\n\tai_socktype: %s\n\tai_protocol: %s\n\t%s\n\n", ai_family_str, ai_socktype_str, ai_protocol_str, socket_addr_str);
    }
  }
  return (const char*)buffer;

}

void test_client_getaddrinfo() {
  int status;
  struct addrinfo hints;
  struct addrinfo *serverinfo; // will point to the results

  memset(&hints, 0, sizeof hints); // make sure struct is empty
  hints.ai_family = AF_UNSPEC;      // don't care for IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;  // TCP stream sockets

  //get ready to connect

  if ((status = getaddrinfo("google.com.ar", NULL, &hints, &serverinfo)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
  }

  printf("serverinfo: %s\n", stringify_serverinfo(serverinfo));

  freeaddrinfo(serverinfo);
}

void test_server_getaddrinfo() {
  int status;
  struct addrinfo hints;
  struct addrinfo *serverinfo; // will point to the results

  memset(&hints, 0, sizeof hints); // make sure struct is empty
  hints.ai_family = AF_UNSPEC;      // don't care for IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;  // TCP stream sockets
  hints.ai_flags = AI_PASSIVE;      // fill in my IP for me

  if ((status = getaddrinfo(NULL, "3490", &hints, &serverinfo)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
  }

  printf("serverinfo: %s\n", stringify_serverinfo(serverinfo));

  freeaddrinfo(serverinfo);
}


int main(int argc, char *argv[])
{
  test_sockaddr();
  test_server_getaddrinfo(); 
  test_client_getaddrinfo(); 
  start_showip_example(argc, argv);
  return 0;
}
