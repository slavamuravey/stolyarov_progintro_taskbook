.PHONY: all
all: clean client server

.PHONY: clean
clean: clean_client clean_server
	@echo "+ $@"

client: clean_client
	@echo "+ $@"	
	@gcc -Wall -g client.c -o client

client.o: client.c
	@echo "+ $@"
	@gcc -Wall -g -c client.c

.PHONY: clean_client
clean_client:
	@echo "+ $@"
	@rm -f client client.o

server: clean_server
	@echo "+ $@"	
	@gcc -Wall -g server.c -o server

server.o: server.c
	@echo "+ $@"
	@gcc -Wall -g -c server.c

.PHONY: clean_server
clean_server:
	@echo "+ $@"
	@rm -f server server.o
