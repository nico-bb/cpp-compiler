FLAGS=-g -O0 -std=c++20 -Wall -Wshadow -Wpedantic
INC=-Isrc

compiler:
	clang++ src/*.cpp ${FLAGS}  ${INC} -o bin/compiler