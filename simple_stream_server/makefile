CC = gcc
CFLAGS = -Wall -I./include/
SOURCES = ./src/*.c 

all: compile

compile: 
	$(CC) $(SOURCES) $(CFLAGS) -o stream_socket

run:
	./stream_socket 

clean:
	rm stream_socket
