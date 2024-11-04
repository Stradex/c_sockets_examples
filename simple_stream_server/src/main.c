#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <client.h>
#include <server.h>

#define DEFAULT_PORT  10666

void invalid_launch_params() {
    fprintf(stderr, "usage:\n\tjoin server: stream_socket join hostname <<port: default 10666>>\n\thost server: stream_socket host <<port: default 10666>>\n"); 
}

/*
* USAGE:
*   join server: ./stream_socket join hostname (optional port)
*   start server: ./stream_socket host (optional port)
*/

int main(int argc, char *argv[])
{
  if (argc < 2) {
    invalid_launch_params();
    return 2;
  }
  if (strcmp(argv[1], "join") == 0) {
    start_client(argv[2], argc >= 4 ? atoi(argv[3]) : DEFAULT_PORT);
  } else if (strcmp(argv[1], "host") == 0) {  
    start_server(argc >= 3 ? atoi(argv[2]) : DEFAULT_PORT);
  } else {
    invalid_launch_params();
    return 2;
  }  
  return 0;
}
