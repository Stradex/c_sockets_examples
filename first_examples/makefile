CC = gcc
CFLAGS = -Wall -I./include/
SOURCES = ./src/*.c 

all: compile run clean

compile: 
	$(CC) $(SOURCES) $(CFLAGS) -o sockets

run:
	./sockets google.com.ar

clean:
	rm sockets
