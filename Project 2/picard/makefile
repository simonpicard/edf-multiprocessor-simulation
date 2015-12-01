CC=mpiCC
CFLAGS=-W -Wall -ansi -pedantic -std=c++0x
LDFLAGS=

all: gameOfLife

gameOfLife: main.o
	$(CC) -o dist/gameOfLife bin/main.o $(LDFLAGS)

main.o: src/main.cpp
	$(CC) -o bin/main.o -c src/main.cpp $(CFLAGS)

clean:
	rm -rf bin/*.o

mrproper: clean
	rm -rf dist/*
