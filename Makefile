CC=g++

CFLAGS=-std=c++17
LDFLAGS=-Wall
SRCS=$(shell find src -name '*.cpp')
TARGET=bin/app

INCLUDES_PATH=include
LIBS=-lGL -lglfw -lGLEW -lGLU

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) -I $(INCLUDES_PATH) $(LIBS)
	./$(TARGET)