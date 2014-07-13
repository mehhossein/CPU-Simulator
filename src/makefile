all: cpu
cpu: main.o flag.o memory.o decoder.o cpu.o 
	g++ main.o flag.o memory.o decoder.o cpu.o -o ../bin/cpu
main.o: main.cpp flag.h cpu.h memory.h
	g++ -c main.cpp 
flag.o: flag.cpp flag.h
	g++ -c flag.cpp
memory.o: memory.cpp memory.h
	g++ -c memory.cpp
decoder.o: decoder.cpp decoder.h memory.h cpu.h 
	g++ -c decoder.cpp
cpu.o: cpu.cpp cpu.h 
	g++ -c cpu.cpp
clean:
	rm -rf *o cpu
