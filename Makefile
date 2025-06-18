# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Wall -Wextra -g

# Linker flags (Linux/macOS)
LDFLAGS = -lssl -lcrypto

# Source files
SRC = src/main.cpp \
      src/commands/add.cpp \
      src/commands/commit.cpp \
	  src/core/commit.cpp\
      src/commands/log.cpp \
      src/utils/hashing.cpp \
      src/core/blob.cpp\
	  src/core/repository.cpp\
	  src/commands/init.cpp\
	  src/utils/file_io.cpp\
	src/commands/checkout.cpp\
	src/commands/branch.cpp\
	src/core/branch.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Executable name
TARGET = minigit

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build
clean:
	rm -f $(OBJ) $(TARGET)

# Run sample tests
test: $(TARGET)
	@echo "Running sample tests..."
	@./$(TARGET) init
	@touch testfile.txt
	@echo "Hello MiniGit!" > testfile.txt
	@./$(TARGET) add testfile.txt
	@./$(TARGET) commit -m "Test Commit"
	@./$(TARGET) log
	@./$(TARGET) branch
	@./$(TARGET) branch test-branch
	@./$(TARGET) checkout test-branch
	@echo "Tests done."

.PHONY: all clean test
