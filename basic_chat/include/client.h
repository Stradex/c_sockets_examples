#ifndef CLIENT_H
#define CLIENT_H

#define MAXDATASIZE   100
#define MAXINPUTSIZE  64
#define POLL_STDIN    0
#define POLL_SERVER   1
#define MAX_POLLS     2

void start_client(const char* hostname, const char* username, int port);

#endif
