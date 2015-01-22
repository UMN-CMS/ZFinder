# Pull in ROOT
ROOT_INCLUDES=`root-config --cflags`
ROOT_ALL=`root-config --cflags --libs`

#Compiler
CC=g++ -O2 -g -std=c++0x -Wall
CCC=${CC} -c

all: plot

plot: efficiency_turn_on.exe
	./efficiency_turn_on.exe

efficiency_turn_on.exe: efficiency_turn_on.cpp
	${CC} ${ROOT_ALL} -o efficiency_turn_on.exe \
	efficiency_turn_on.cpp


# All .o files rely on the .cpp and .h files
%.o:%.cpp %.h
	${CCC} ${ROOT_INCLUDES} $< -o $@

clean:
	rm -f efficiency_turn_on.exe *.o
