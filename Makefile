.PHONY: build debug

build:
	g++ Point.cpp Line.cpp dump.cpp primitives.cpp StraightSkeleton.cpp main.cpp -std=c++17 -Wall -O2 -o skeleton

debug:
	g++ Point.cpp Line.cpp dump.cpp primitives.cpp StraightSkeleton.cpp main.cpp -std=c++17 -Wall -fsanitize=address -fsanitize=undefined -g3 -o skeleton
