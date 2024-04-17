CC = gcc
CFLAGS = -ansi -Wall -pedantic

# Source files
SRC = Assembler.c \
      tree.c \
      error_checker.c \
      pre_assembler.c \
      syntax_pass.c \
      set_tree.c \
      set_code.c \
      creat_files.c \
      general.c \
      second_pass.c

# Object files
OBJ = $(SRC:.c=.o)

# Targets
TARGET = assembler

# Default target
all: $(TARGET)

# Compile each source file into an object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into the final executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

# Clean up object files and the executable
clean:
	rm -f $(OBJ) $(TARGET)
