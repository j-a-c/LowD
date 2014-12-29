.PHONY: lowd

lowd: linux

linux:
	g++ main.cpp -O2 -std=c++11 -lGLU -lGLEW -lglfw -o bin/lowd
#	Optimization and gprof
#	g++ -pg -O2 cube.cpp -lGLU -lGLEW -lglfw -o cube
