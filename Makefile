# Compiler and flags
CXX = clang++
CXXFLAGS = -Wall -I/opt/homebrew/include/SDL2
LDFLAGS = -L/opt/homebrew/lib -lSDL2

# Output executable
TARGET = main

# Source and object files
SRC  = $(wildcard src/**/*.cpp) $(wildcard src/*.cpp)
OBJ  = $(SRC:.cpp=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compile each source file to an object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJ) $(TARGET)

# Rebuild everything
rebuild: clean all
