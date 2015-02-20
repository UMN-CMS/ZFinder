# Pull in ROOT
ROOT_INCLUDES=`root-config --cflags`
ROOT_ALL=`root-config --cflags --libs`

#Compiler
CC=g++ -O2 -g -std=c++0x -Wall
CCC=${CC} -c

all: tuple_to_histogram.exe

tuple_to_histogram.exe: tuple_to_histogram.cpp
	${CC} ${ROOT_ALL} -o tuple_to_histogram.exe \
	tuple_to_histogram.cpp

clean:
	rm -f tuple_to_histogram.exe *.o
