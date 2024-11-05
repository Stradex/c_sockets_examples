CC = gcc
CFLAGS = -Wall -I./include/
SOURCES = ./src/*.c 

all: compile

compile: 
	$(CC) $(SOURCES) $(CFLAGS) -o datagram_socket

run:
	./datagram_socket 

clean:
	rm datagram_socket
