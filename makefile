CC = g++ -std=c++11
CCFLAGS += -l SDL2 -l SDL2_image -l SDL2_ttf -l SDL2_mixer

all : main.out

main.out : main.o engine.o map.o gui.o entity.o env.o rsdl.o
	$(CC) main.o engine.o map.o gui.o entity.o env.o rsdl.o $(CCFLAGS) -o main.out

main.o :  main.cpp src/engine.hpp src/engine.cpp src/entity.hpp src/map.hpp src/gui.hpp src/env.hpp src/sdl/rsdl.hpp
	$(CC) -c main.cpp -o main.o

engine.o : src/engine.hpp src/engine.cpp src/entity.hpp src/map.hpp src/gui.hpp src/env.hpp src/sdl/rsdl.hpp
	$(CC) -c src/engine.cpp -o engine.o

gui.o : src/gui.hpp src/gui.cpp src/entity.hpp src/env.hpp src/sdl/rsdl.hpp
	$(CC) -c src/gui.cpp -o gui.o

map.o : src/map.hpp src/map.cpp src/entity.hpp src/env.hpp src/sdl/rsdl.hpp
	$(CC) -c src/map.cpp -o map.o

entity.o : src/entity.hpp src/entity.cpp src/env.hpp src/sdl/rsdl.hpp
	$(CC) -c src/entity.cpp -o entity.o

env.o : src/env.hpp src/env.cpp src/sdl/rsdl.hpp
	$(CC) -c src/env.cpp -o env.o

rsdl.o : src/sdl/rsdl.hpp src/sdl/rsdl.cpp
	$(CC) -c src/sdl/rsdl.cpp -o rsdl.o

clean : 
	rm *.o main.out