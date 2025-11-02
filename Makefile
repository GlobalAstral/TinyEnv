CXX = gcc
CXXFLAGS = -I src -I libs\ConsoleUtils\src -std=c17 -g

SRC = .\src\\*.c .\libs\ConsoleUtils\src\ConsoleUtils\\*.c
EXEC = TinyEnv.exe

all: build

build:
	$(CXX) $(SRC) $(CXXFLAGS) -o $(EXEC)
