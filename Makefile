CC = g++
CFLAGS = -g -Wall -std=c++17
SRC = $(wildcard src/*.cpp src/*/*.cpp)
OUT = bin/main

.PHONY: all build run clean

all: build run

build:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run:
	./$(OUT)

clean:
	del /f /q bin\main.exe 2>nul || exit 0
