-include ../../include/main.mk

create_numbers: clean_create_numbers
	@echo "+ $@"	
	@gcc -Wall -g create_numbers.c -o create_numbers

.PHONY: clean
clean: clean_main clean_create_numbers
	@echo "+ $@"

.PHONY: clean_create_numbers
clean_create_numbers:
	@echo "+ $@"
	@rm -f create_numbers create_numbers.o