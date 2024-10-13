CXX = clang++
CXXFLAGS = -Wall -I/opt/homebrew/include/SDL2
LDFLAGS = -L/opt/homebrew/lib -lSDL2

SRC  = $(wildcard src/**.cpp)
OBJ  = $(patsubst %.cpp, $(BIN)/%.o, $(SRC))
BIN = bin

all: dirs game

dirs:
	mkdir -p $(BIN)
	mkdir -p $(BIN)/src

run: all
	$(BIN)/game

game: $(OBJ)
	$(CXX) -o $(BIN)/game $^ $(LDFLAGS)

$(BIN)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN)

rebuild: clean all