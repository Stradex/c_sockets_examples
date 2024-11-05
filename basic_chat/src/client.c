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



void start_client(const char* hostname, int port)
{
  printf("[client] connecting to %s:%d...\n", hostname, port);
}
