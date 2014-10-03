# Pull in ROOT
ROOT_INCLUDES=`root-config --cflags`
ROOT_ALL=`root-config --cflags --libs`

#Compiler
CC=g++ -O2 -g -std=c++0x -Wall
CCC=${CC} -c

all: total_cross_section.exe

total_cross_section.exe: total_cross_section.cpp
	${CC} ${ROOT_ALL} -o total_cross_section.exe \
	total_cross_section.cpp


# All .o files rely on the .cpp and .h files
%.o:%.cpp %.h
	${CCC} ${ROOT_INCLUDES} $< -o $@

clean:
	rm -f total_cross_section.exe *.o
