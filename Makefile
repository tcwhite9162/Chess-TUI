CXX = g++
C_FLAGS = -std=c++20 -Wall -Wextra -O2

L_FLAGS = -lncurses -lncursesw

SRC_DIR = src
BUILD_DIR = build
BIN = tui

SRCS = $(shell find $(SRC_DIR) -name '*.cpp')

OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(L_FLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(C_FLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN)

run: $(BIN)
	./$(BIN)

.PHONY: all clean run
