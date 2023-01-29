mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

.PHONY: all
all: clean main
	@./main

main: atoi.o itoa.o get_number.o print_bytes.o main.o
	@echo "+ $@"
	@ld -m elf_i386 atoi.o itoa.o get_number.o print_bytes.o main.o -o main

main.o: main.asm
	@echo "+ $@"
	@nasm -felf main.asm -o main.o

atoi.o: atoi.asm
	@echo "+ $@"
	@nasm -felf atoi.asm -o atoi.o

itoa.o: itoa.asm
	@echo "+ $@"
	@nasm -felf itoa.asm -o itoa.o

get_number.o: get_number.asm
	@echo "+ $@"
	@nasm -felf get_number.asm -o get_number.o

print_bytes.o: print_bytes.asm
	@echo "+ $@"
	@nasm -felf print_bytes.asm -o print_bytes.o

.PHONY: clean
clean:
	@echo "+ $@"
	@rm -f atoi.o itoa.o get_number.o print_bytes.o main.o main

