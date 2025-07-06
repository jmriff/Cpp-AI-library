# Define compiler and flags
CC       = gcc
CXX      = g++
CFLAGS   = -fPIC -std=c11  #-Wall -Wextra
CXXFLAGS = -fPIC -std=c++11 #-Wall -Wextra

# Define directories
BUILD_DIR = build
SRC_DIR   = src
INC_DIR   = include

USR_INCLUDE = /usr/local/include
USR_LIB     = /usr/local/lib

AI = /usr/local/include/AI

# Define source files and object files
C_SRCS   = $(wildcard $(SRC_DIR)/*.c)
CXX_SRCS = $(wildcard $(SRC_DIR)/*.cpp)
C_OBJS   = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.c.o,$(C_SRCS))
CXX_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.cpp.o,$(CXX_SRCS))
OBJS     = $(C_OBJS) $(CXX_OBJS)

# Includes
INCS = $(INC_DIR/*:$(INC_DIR)/%.h=%.h)

# Define library name
LIB_NAME = $(BUILD_DIR)/libAI.so

# Colors
RED    = \033[31m
YELLOW = \033[33m
GREEN  = \033[32m
BLUE   = \033[34m

BOLD  = \033[1m
RESET = \033[0m

.PHONY: all clean install uninstall

all: $(LIB_NAME)

# Rule to create the build directory
$(BUILD_DIR):
	clear
	@mkdir -p $(BUILD_DIR)

# Rule to compile C source files into object files
$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@printf "$(GREEN)Building C   object $@..."
	@$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@
	@printf "\tDone $(RESET)\n"

# Rule to compile C++ source files into object files
$(BUILD_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@printf "$(BLUE)Building CXX object $@..."
	@$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@
	@printf "\t\tDone $(RESET)\n"

# Rule to create the static library
$(LIB_NAME): $(OBJS)
	@printf "$(RED)$(BOLD)Linking objects into $@..."
	@$(CXX) $(CXXFLAGS) -shared -o $(LIB_NAME) $(OBJS)
	@printf "\t\tDone\n$(RESET)"

# Test build variables
test: $(LIB_NAME) $(C_OBJS)
	@if [ -z "$(TEST)" ]; then \
		echo "Please specify a test to build with TEST=<name> (without .cpp)"; \
		exit 1; \
	fi; \
	$(CXX) $(CXXFLAGS) tests/$(TEST).cpp $(C_OBJS) -lAI -o $(BUILD_DIR)/$(TEST).test
	@./build/$(TEST).test || { \
		echo "Test failed"; \
		exit 1; \
	}

# Install library
install:
	mkdir -p $(AI)
	cp $(INC_DIR)/* $(AI)/
	cp $(LIB_NAME) $(USR_LIB)/
	ldconfig

# Remove library
uninstall: $(INCS)
	rm -rf $(AI) $(USR_LIB)/$(LIB_NAME)

# Clean rule to remove build directory and library
clean:
	@rm -rf $(BUILD_DIR) $(USR_LIB)/$(LIB_NAME)

help:
	@echo "Usage:"
	@echo "  make all or make      - Build the library"
	@echo "  make clean            - Clean build files"
	@echo "  make install          - Install the library"
	@echo "  make uninstall        - Uninstall the library"
	@echo "  make test TEST=<name> - Build and run a test (without .cpp extension)"
	@echo "  make help             - Show this help message"