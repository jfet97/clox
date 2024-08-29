# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g
SRC = main.c debug.c chunk.c memory.c value.c vm.c
TARGET = program

# Default target
all: $(TARGET)

# Linking the target
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Clean up
clean:
	rm -f $(TARGET)
	rm -rf $(TARGET).dSYM
