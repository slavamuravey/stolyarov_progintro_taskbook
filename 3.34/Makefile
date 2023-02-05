mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

.PHONY: all
all: clean main
	@./main

main: strlen.o strchar.o main.o
	@echo "+ $@"
	@ld -m elf_i386 strlen.o strchar.o main.o -o main

main.o: main.asm
	@echo "+ $@"
	@nasm -felf main.asm -o main.o

strlen.o: strlen.asm
	@echo "+ $@"
	@nasm -felf strlen.asm -o strlen.o

strchar.o: strchar.asm
	@echo "+ $@"
	@nasm -felf strchar.asm -o strchar.o

.PHONY: clean
clean:
	@echo "+ $@"
	@rm -f strlen.o strchar.o main.o main
