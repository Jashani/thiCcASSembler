CC = gcc
FLAGS = -ansi -Wall -pedantic

main: common.o main.c main.h
	$(CC) -g common.o main.c $(FLAGS) -o $@

common.o: common.c common.h
	$(CC) -c common.c $(CFLAGS) -o $@