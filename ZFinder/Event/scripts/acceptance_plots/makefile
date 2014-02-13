# Pull in ROOT
ROOT_INCLUDES=`root-config --cflags`
ROOT_ALL=`root-config --cflags --libs`

#Compiler
CC=g++ -O2

all: make_acceptance_plots.exe

make_acceptance_plots.exe: make_acceptance_plots.cpp
	${CC} -o make_acceptance_plots.exe make_acceptance_plots.cpp ${ROOT_ALL}

clean:
	rm make_acceptance_plots.exe
