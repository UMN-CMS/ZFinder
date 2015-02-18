# Pull in ROOT
ROOT_INCLUDES=`root-config --cflags`
ROOT_ALL=`root-config --cflags --libs`

#Compiler
CC=g++ -O2 -g -std=c++0x -Wall
CCC=${CC} -c

all: same_sign.exe

same_sign.exe: same_sign.cpp
	${CC} ${ROOT_ALL} -o same_sign.exe \
	same_sign.cpp

clean:
	rm -f same_sign.exe *.o
