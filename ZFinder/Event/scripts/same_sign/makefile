# Pull in ROOT
ROOT_INCLUDES=`root-config --cflags`
ROOT_ALL=`root-config --cflags --libs`
ROO_INCLUDES=${ROOT_ALL} -I${ROOFITSYS}/include -L${ROOFITSYS}/lib -lRooFit -lRooFitCore -lRooStats

#Compiler
CC=g++ -O2 -g -std=c++0x -Wall
CCC=${CC} -c

all: same_sign.exe

same_sign.exe: same_sign.cpp same_sign.h FitFunction.o PlotStyle.o
	${CC} ${ROOT_ALL} ${ROO_INCLUDES} -o same_sign.exe \
	same_sign.cpp \
	FitFunction.o \
	PlotStyle.o

%.o:%.cpp %.h
	${CCC} ${ROOT_INCLUDES} $< -o $@

clean:
	rm -f same_sign.exe *.o
