# Pull in ROOT
ROOT_INCLUDES=`root-config --cflags`
ROOT_ALL=`root-config --cflags --libs`

#Compiler
CC=g++ -O2 -g -std=c++0x -Wall
CCC=${CC} -c

all: cut_by_cut_plotter.exe

cut_by_cut_plotter.exe: cut_by_cut_plotter.cpp
	${CC} ${ROOT_ALL} -o cut_by_cut_plotter.exe \
	cut_by_cut_plotter.cpp


# All .o files rely on the .cpp and .h files
%.o:%.cpp %.h
	${CCC} ${ROOT_INCLUDES} $< -o $@

clean:
	rm -f cut_by_cut_plotter.exe *.o
