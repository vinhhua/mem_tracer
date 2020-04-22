CC = gcc
CFLAGS = -Wall -Werror
OBJFILES = mem_tracer.c CommandNode.c
TARGET = mem_tracer

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -g -o $(TARGET) $(OBJFILES)


clean:
	rm -f $(OBJECTS) $(TARGET) *~
