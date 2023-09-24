.PHONY: all
all: clean main
	@./main

main: clean main.o start.o syscall.o
	@echo "+ $@"	
	@ld -m elf_i386 main.o start.o syscall.o -o main

main.o: main.c
	@echo "+ $@"
	@gcc -fno-stack-protector -m32 -Wall -g -c main.c

start.o: start.asm
	@echo "+ $@"
	@nasm -f elf start.asm

syscall.o: syscall.asm
	@echo "+ $@"
	@nasm -f elf syscall.asm

.PHONY: clean
clean:
	@rm -f main *.o
