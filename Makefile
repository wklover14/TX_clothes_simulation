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
CFLAGS = -Wall -I$(INCLUDE_DIR)

# Linker flags
LDFLAGS = -lm

# Memory checker
MEMCHECKER = valgrind
MEMFLAGS = --leak-check=full --track-origins=yes -s

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Default target
all: clean run

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

# Run the application
run-rideau: $(TARGET)
	@echo ""
	@echo ""
	./$(TARGET) curtain

run-nappe: $(TARGET)
	@echo ""
	@echo ""
	./$(TARGET) table-cloth

run-tissus: $(TARGET)
	@echo ""
	@echo ""
	./$(TARGET) soft

# Run the application with memory check
saferun-rideau: $(TARGET)
	@echo ""
	$(MEMCHECKER) $(MEMFLAGS) ./$(TARGET) curtain

saferun-nappe: $(TARGET)
	@echo ""
	$(MEMCHECKER) $(MEMFLAGS) ./$(TARGET) table-cloth

saferun-tissus: $(TARGET)
	@echo ""
	$(MEMCHECKER) $(MEMFLAGS) ./$(TARGET) soft

# Add phony targets
.PHONY: all clean run
