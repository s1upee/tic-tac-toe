# Compiler and flags
CC = gcc
CFLAGS = -lncurses -Wall -Wextra -pedantic

# Target executable
TARGET = tic_tac_toe

# Source files
SRCS = tic_tac_toe.c

# Default rule
all: $(TARGET)

# Rule to build the target
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Rule to clean up
clean:
	rm -f $(TARGET)

# Optional rule for testing the game
run: $(TARGET)
	./$(TARGET)