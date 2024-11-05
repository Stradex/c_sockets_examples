#ifndef NETWORK_H
#define NETWORK_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>


void* get_in_addr(struct sockaddr *sa);

const char* port_as_str(const int port);

#endif
