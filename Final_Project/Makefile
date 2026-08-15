# Compiler and required flags: -Wall -ansi -pedantic must all be present
# so the build must compile cleanly with no warnings under these flags.
CC      = gcc
CFLAGS  = -Wall -ansi -pedantic -g

# Project directories and final executable name
SRC_DIR = ./Sources
OBJ_DIR = build
TARGET  = assembler

# All .c files under Sources/
SOURCES = $(wildcard $(SRC_DIR)/*.c)

# Map each source file to its corresponding object file under build/
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

# All headers; listed as a dependency so changing a header triggers a rebuild
HEADERS = $(wildcard ./Headers/*.h)

# Link all object files into the final executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

# Compile each source file into its object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Make sure the build directory exists before any object file is built
$(OBJECTS): | $(OBJ_DIR)
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Remove the build directory, the compiled executable, and any test-generated output files
clean:
	rm -rf $(OBJ_DIR)
	rm -f $(TARGET)
	find Tests -type f \( -name "*.am" -o -name "*.ob" -o -name "*.ent" -o -name "*.ext" \) -delete