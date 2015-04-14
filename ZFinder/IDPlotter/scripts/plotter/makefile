# Pull in ROOT
ROOT_INCLUDES=`root-config --cflags`
ROOT_ALL=`root-config --cflags --libs`

#Compiler
CC=g++ -O2 -g -std=c++0x -Wall
CCC=${CC} -c

all: makeplots

plotter.exe: plotter.cpp
	${CC} ${ROOT_ALL} -o plotter.exe \
	plotter.cpp

makeplots: plotter.exe
	./plotter.exe

clean:
	rm -f plotter.exe
