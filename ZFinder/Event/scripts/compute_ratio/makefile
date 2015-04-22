# Pull in ROOT
ROOT_INCLUDES=`root-config --cflags`
ROOT_ALL=`root-config --cflags --libs`

#Compiler
CC=g++ -O2 -g -std=c++0x -Wall
CCC=${CC} -c

all: compute_ratio.exe

compute_ratio.exe: compute_ratio.cpp
	${CC} ${ROOT_ALL} ${ROO_INCLUDES} -o compute_ratio.exe \
		compute_ratio.cpp

clean:
	rm -f compute_ratio.exe
