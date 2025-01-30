#CXXFLAGS = -g -I../../../utils
CXXFLAGS = -O3 -I../../../utils

all:
	g++ $(CXXFLAGS) -c ../../../utils/Stopwatch.cpp
	g++ $(CXXFLAGS) main.cpp -o main Stopwatch.o

clean:
	rm -f *.o
	rm -f main

