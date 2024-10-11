# Compiler and flags
CXX = clang++
CXXFLAGS = -Wall -I/opt/homebrew/include/SDL2
LDFLAGS = -L/opt/homebrew/lib -lSDL2

# Output executable
TARGET = main

# Object files
OBJ = main.o

# Build the executable
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compile the source files
$(OBJ): main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o $(OBJ)

# Clean up generated files
clean:
	rm -f $(OBJ) $(TARGET)

# Rebuild everything
rebuild: clean all
