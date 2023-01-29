mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

.PHONY: all
all: clean main
	@./main

main: clean_main main.o
	@echo "+ $@"	
	@ld -m elf_i386 main.o -o main

main.o: main.asm
	@echo "+ $@"
	@nasm -felf main.asm -o main.o

main_debug: clean_main_debug main_debug.o
	@echo "+ $@"	
	@ld -m elf_i386 main_debug.o -o main_debug

main_debug.o: main.asm
	@echo "+ $@"
	@nasm -g -felf main.asm -o main_debug.o

.PHONY: debug
debug: main_debug
	@echo "+ $@"
	gdb -x $(mkfile_dir)/main.gdb

.PHONY: clean
clean: clean_main clean_main_debug
	@echo "+ $@"

.PHONY: clean_main
clean_main:
	@echo "+ $@"
	@rm -f main main.o

.PHONY: clean_main_debug
clean_main_debug:
	@echo "+ $@"
	@rm -f main_debug main_debug.o
