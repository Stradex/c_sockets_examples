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
#include <client.h>
#include <network.h>
#include <server.h>


/***
* USAGE:
* ./chat host [port=10666]
* ./chat join hostname [port=10666]
*/
void invalid_args() {
  fprintf(stderr, "usage:\n./chat host [port=10666]\n./chat join hostname [port=10666]\n");
}
int main(int argc, char* argv[]) {
  if (argc < 2) {
    invalid_args();
    return 1; 
  } 
  if (strcmp(argv[1], "host") == 0) {
    start_server(argc >= 3 ? atoi(argv[2]) : DEFAULT_PORT);
  } else if (strcmp(argv[1], "join") == 0) {
    if (argc < 3) { 
      invalid_args();
      return 1;
    }
    start_client(argv[2], argc >= 4 ? atoi(argv[3]) : DEFAULT_PORT);
  }
}

