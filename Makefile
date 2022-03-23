CC = gcc
FLAGS = -ansi -Wall -pedantic
O_FILES = utilities.o errors.o macro_table.o symbol_table.o term_detection.o machine_code.o pre_assembly.o first_pass.o second_pass.o

assembler: $(O_FILES) assembler.c assembler.h
	$(CC) -g $(O_FILES) assembler.c $(FLAGS) -o $@

second_pass.o: second_pass.c second_pass.h
	$(CC) -c second_pass.c $(CFLAGS) -o $@

first_pass.o: first_pass.c first_pass.h
	$(CC) -c first_pass.c $(CFLAGS) -o $@

pre_assembly.o: pre_assembly.c pre_assembly.h
	$(CC) -c pre_assembly.c $(CFLAGS) -o $@

machine_code.o: machine_code.c machine_code.h
	$(CC) -c machine_code.c $(CFLAGS) -o $@

term_detection.o: term_detection.c term_detection.h
	$(CC) -c term_detection.c $(CFLAGS) -o $@

symbol_table.o: symbol_table.c symbol_table.h
	$(CC) -c symbol_table.c $(CFLAGS) -o $@

macro_table.o: macro_table.c macro_table.h
	$(CC) -c macro_table.c $(CFLAGS) -o $@

errors.o: errors.c errors.h
	$(CC) -c errors.c $(CFLAGS) -o $@

utilities.o: utilities.c utilities.h common.h
	$(CC) -c utilities.c $(CFLAGS) -o $@
