CC = gcc
CFLAGS = -Wall -I./include/
SOURCES = ./src/*.c 

all: compile

compile: 
	$(CC) $(SOURCES) $(CFLAGS) -o basic_poll

run:
	./basic_poll 

clean:
	rm basic_poll
