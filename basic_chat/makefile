CC = gcc
CFLAGS = -Wall -I./include/
SOURCES = ./src/*.c 

all: compile

compile: 
	$(CC) $(SOURCES) $(CFLAGS) -o chat

run:
	./chat 

clean:
	rm chat
