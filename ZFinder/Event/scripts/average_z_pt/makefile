# Pull in ROOT
ROOT_INCLUDES=`root-config --cflags`
ROOT_ALL=`root-config --cflags --libs`

#Compiler
CC=g++ -O2 -g -std=c++0x -Wall
CCC=${CC} -c

all: average_z_pt.exe

average_z_pt.exe: average_z_pt.cpp
	${CC} ${ROOT_ALL} ${ROO_INCLUDES} -o average_z_pt.exe \
		average_z_pt.cpp

clean:
	rm -f average_z_pt.exe
