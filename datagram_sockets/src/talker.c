#include <talker.h>

#include <stdio.h>

void start_talker(const char* hostname, const int port, const char* message)
{
  printf("starting talker: %s:%d - %s...\n", hostname, port, message);
}
