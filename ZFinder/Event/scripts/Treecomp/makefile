# Pull in ROOT
ROOT_INCLUDES=`root-config --cflags`
ROOT_ALL=`root-config --cflags --libs`
ROO_INCLUDES=${ROOT_ALL} -I${ROOFITSYS}/include -L${ROOFITSYS}/lib -lRooFit -lRooFitCore -lRooStats

#Compiler
CC=g++ -O2 -g -std=c++0x -Wall
CCC=${CC} -c

all: TreeComp.exe

TreeComp.exe: TreeComp.cpp
	${CC} ${ROOT_ALL} ${ROO_INCLUDES} -o TreeComp.exe \
	TreeComp.cpp 

%.o:%.cpp %.h
	${CCC} ${ROOT_INCLUDES} $< -o $@

clean:
	rm -f TreeComp.exe *.o
