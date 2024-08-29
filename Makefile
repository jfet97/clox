# Variables
CC = clang
CFLAGS = -Wall  -g
SRC = main.c debug.c chunk.c memory.c
TARGET = program

# Default target
all: $(TARGET)

# Linking the target
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Clean up
clean:
	rm -f $(TARGET)
