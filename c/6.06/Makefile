.PHONY: all
all: clean send recv

.PHONY: clean
clean: clean_send clean_recv
	@echo "+ $@"

send: clean_send
	@echo "+ $@"	
	@gcc -Wall -g send.c -o send

send.o: send.c
	@echo "+ $@"
	@gcc -Wall -g -c send.c

.PHONY: clean_send
clean_send:
	@echo "+ $@"
	@rm -f send send.o

recv: clean_recv
	@echo "+ $@"	
	@gcc -Wall -g recv.c -o recv

recv.o: recv.c
	@echo "+ $@"
	@gcc -Wall -g -c recv.c

.PHONY: clean_recv
clean_recv:
	@echo "+ $@"
	@rm -f recv recv.o
