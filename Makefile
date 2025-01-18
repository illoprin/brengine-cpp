CC=g++

CFLAGS=-std=c++17 -Wall -g
LDFLAGS=-lm -lGL -lglfw -lGLEW -lGLU

SRC_DIR=src
BIN_DIR=bin

TARGET=app

SRCS:=$(shell find $(SRC_DIR) -name '*.cpp')
OBJS:=$(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(SRCS))

INCLUDES = -I include -I $(SRC_DIR)

TEST_SRC?=_test/parse_blf.cpp
TEST_TARGET=test

all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJS) | $(BIN_DIR)
	@echo Linking...
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	@mkdir -p $(dir $@)
	@echo Compiling $<...
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BIN_DIR):
	mkdir -p $@

clean:
	rm -rdf $(BIN_DIR)

clean_tmp:
	rm -drf tmp screenshots

run: $(BIN_DIR)/$(TARGET)
	./$<

# Make bin file for tests
test:
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(BIN_DIR)/$(TEST_TARGET) $(INCLUDES) $(LDFLAGS)
	./$(BIN_DIR)/$(TEST_TARGET)