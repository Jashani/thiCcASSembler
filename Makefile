CC = gcc
FLAGS = -ansi -Wall -pedantic

main: common.o errors.o macro_table.o symbol_table.o pre_assembly.o first_pass.o main.c main.h
	$(CC) -g common.o errors.o macro_table.o symbol_table.o pre_assembly.o first_pass.o main.c $(FLAGS) -o $@

first_pass.o: first_pass.c first_pass.h
	$(CC) -c first_pass.c $(CFLAGS) -o $@

pre_assembly.o: pre_assembly.c pre_assembly.h
	$(CC) -c pre_assembly.c $(CFLAGS) -o $@

symbol_table.o: symbol_table.c symbol_table.h
	$(CC) -c symbol_table.c $(CFLAGS) -o $@

macro_table.o: macro_table.c macro_table.h
	$(CC) -c macro_table.c $(CFLAGS) -o $@

errors.o: errors.c errors.h
	$(CC) -c errors.c $(CFLAGS) -o $@

common.o: common.c common.h
	$(CC) -c common.c $(CFLAGS) -o $@