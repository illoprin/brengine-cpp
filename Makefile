CC=g++

CFLAGS=-std=c++17 -Wall -g
LDFLAGS=-lGL -lglfw -lGLEW -lGLU

SRC_DIR=src
OBJ_DIR=tmp
BIN_DIR=bin

TARGET=app

SRCS:=$(shell find $(SRC_DIR) -name '*.cpp')
OBJS:=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

INCLUDES = -I include

all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJS) | $(BIN_DIR)
	@echo Linking...
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@echo Compiling $<...
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

$(BIN_DIR):
	@mkdir $@

clean:
	rm -rdf $(OBJ_DIR) $(BIN_DIR) screenshots

run: $(BIN_DIR)/$(TARGET)
	./$<