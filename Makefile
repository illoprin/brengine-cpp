CC=g++

CFLAGS=-std=c++17
LDFLAGS=-Wall
SRCS=$(shell find src -name '*.cpp')
TARGET=bin/app

INCLUDES_PATH=include
LIBS=-lGL -lglfw -lGLEW -lGLU

TEST_SRC?=parse_obj.cpp

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) -I $(INCLUDES_PATH) $(LIBS)
	./$(TARGET)

clear:
	rm -f tmp/* bin/*

test:
	$(CC) $(CFLAGS) _test/$(TEST_SRC) -o bin/test -I $(INCLUDES_PATH) $(LIBS)
	./bin/test
	