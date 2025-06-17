# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Wall -Wextra -g

# Linker flags for OpenSSL
LDFLAGS = -lssl -lcrypto

# Source files
SRC = src/main.cpp \
      src/commands/add.cpp \
      src/utils/hashing.cpp \
      src/core/blob.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Executable name
TARGET = minigit

# Default target
all: $(TARGET)

# Link the executable with OpenSSL
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJ) $(TARGET)

# Run tests (you can customize these commands)
test: $(TARGET)
	@echo "Running sample tests..."
	@./$(TARGET) init
	@touch testfile.txt
	@echo "Hello MiniGit!" > testfile.txt
	@./$(TARGET) add testfile.txt
	@echo "Tests done."

.PHONY: all clean test
