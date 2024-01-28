LIBS ?=

.PHONY: all
all: clean main
	@./main

main: clean_main
	@echo "+ $@"	
	@gcc -Wall -g main.c -o main $(LIBS)

main.o: main.c
	@echo "+ $@"
	@gcc -Wall -g -c main.c

.PHONY: clean
clean: clean_main
	@echo "+ $@"

.PHONY: clean_main
clean_main:
	@echo "+ $@"
	@rm -f main main.o
