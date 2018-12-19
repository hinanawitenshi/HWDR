CC := g++
BUILD := build/
SRC := src/
BIN := bin/
EXE := HWDR
SRCS := $(wildcard $(SRC)*.cpp)
OBJS := $(patsubst $(SRC)%.cpp, $(BUILD)%.o, $(SRCS))
FLAGS := -std=c++11

$(BIN)$(EXE): $(OBJS)
	@mkdir -p $(BIN)
	$(CC) $(FLAGS) $^ -o $@

$(BUILD)%.o: $(SRC)%.cpp
	@mkdir -p $(BUILD)
	$(CC) $(FLAGS) $^ -c -o $@

all: clean $(BIN)$(EXE)

run: all
	@$(BIN)$(EXE)

clean:
	@rm -rf $(BUILD)
	@rm -rf $(BIN)
