# Compiler
CC = gcc

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
INCLUDE_DIR = include
VTK_DIR = vtk_grid* vtk_poly*

# Executable name
TARGET = $(BIN_DIR)/app

# Compiler flags
CFLAGS = -fopenmp -Wall -I$(INCLUDE_DIR)

# Linker flags
LDFLAGS = -lm -fopenmp

# Memory checker
MEMCHECKER = valgrind
MEMFLAGS = --leak-check=full --track-origins=yes -s

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Default target
all: clean build

# Build the target executable
$(TARGET): $(OBJ_FILES)
	mkdir -p $(BIN_DIR)
	$(CC) $(OBJ_FILES) -o $(TARGET) $(LDFLAGS)

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build and bin directories
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) $(VTK_DIR)

# Compile source files and create an executable
build:$(TARGET)
	@echo ""
	@echo "Application created"

# Run the application
run-rideau:
	@echo ""
	@echo ""
	./$(TARGET) curtain

run-nappe:
	@echo ""
	./$(TARGET) table-cloth

run-tissus:
	@echo ""
	./$(TARGET) soft

# Run the application with memory check
saferun-rideau:
	@echo ""
	$(MEMCHECKER) $(MEMFLAGS) ./$(TARGET) curtain

saferun-nappe:
	@echo ""
	$(MEMCHECKER) $(MEMFLAGS) ./$(TARGET) table-cloth

saferun-tissus:
	@echo ""
	$(MEMCHECKER) $(MEMFLAGS) ./$(TARGET) soft

# Add phony targets
.PHONY: all clean build
