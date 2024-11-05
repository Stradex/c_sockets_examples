#include <listener.h>
#include <talker.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_PORT 10666

/***
* USAGE:
* ./datagram_socket listener [port=10666]
* ./datagram_socket talker hostname [port=10666] [message=Hello]
*/
void invalid_args() {
  fprintf(stderr, "usage:\n./datagram_socket listener [port=10666]\n./datagram_socket talker hostname [port=10666] [message=Hello]\n");
}
int main(int argc, char* argv[]) {
  if (argc < 2) {
    invalid_args();
    return 1; 
  } 
  if (strcmp(argv[1], "listener") == 0) {
    start_listener(argc >= 3 ? atoi(argv[2]) : DEFAULT_PORT);
  } else if (strcmp(argv[1], "talker") == 0) {
    if (argc < 3) { 
      invalid_args();
      return 1;
    }
    char message[256] = "";
    if (argc < 5) {
      strcpy(message, "Hello");
    }
    for (int i=4; i < argc; i++) {
      if (i > 4) {
        char buffer[128] = "";
        strcpy(buffer, message);
        sprintf(message, "%s %s", buffer, argv[i]);
      } else {
        sprintf(message, "%s", argv[i]);
      }
    }
    start_talker(argv[2], argc >= 4 ? atoi(argv[3]) : DEFAULT_PORT, (const char*)message);
  }
}
