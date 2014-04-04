# Pull in ROOT
ROOT_INCLUDES=`root-config --cflags`
ROOT_ALL=`root-config --cflags --libs`

#Compiler
CC=g++ -O2 -g
CCC=${CC} -c

all: make_plots.exe

make_plots.exe: make_plots.cpp cross_check_plotter.o
	${CC} ${ROOT_ALL} -o make_plots.exe \
	make_plots.cpp \
	cross_check_plotter.o

# All .o files rely on the .cpp and .h files
%.o:%.cpp %.h
	${CCC} ${ROOT_INCLUDES} $< -o $@

clean:
	rm -f make_plots.exe *.o
