FLAGS=-g -O0 -std=c++20 -Wall -Wshadow -Wpedantic
INC=-Isrc

all: compiler

compiler:
	g++ src/*.cpp ${FLAGS}  ${INC} -o bin/compiler

clean:
	rm -f bin/*.o bin/mini-sim