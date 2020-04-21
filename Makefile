CC = gcc
mem_tracer: mem_tracer.c
	$(CC) -g -o mem_tracer mem_tracer.c -Wall -Werror
