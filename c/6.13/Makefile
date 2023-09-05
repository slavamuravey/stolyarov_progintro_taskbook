CC = gcc
CFLAGS = -g -Wall -ansi -pedantic
SRCMODULES = dynamic_array.c connection.c utils.c server.c
OBJMODULES = $(SRCMODULES:.c=.o)
PORT ?= 8181

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: run
run: clean server
	./server $(PORT)

server: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f *.o server

.PHONY: check_memory_leaks
check_memory_leaks: clean server
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./server $(PORT)
