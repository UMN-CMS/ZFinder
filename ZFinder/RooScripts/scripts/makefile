# Pull in ROOT
ROOT_INCLUDES=`root-config --cflags`
ROOT_ALL=`root-config --cflags --libs`
ROO_INCLUDES=${ROOT_ALL} -I${ROOFITSYS}/include -L${ROOFITSYS}/lib -lRooFit -lRooFitCore -lRooStats

#Compiler
CC=g++ -g -O2
CC11=${CC} -std=gnu++0x
CCC=${CC} -c

all: RooFitter.exe

RooFitter.exe: RooFitter.cpp
	${CC}  \
	${ROO_INCLUDES} \
	-o RooFitter.exe \
	RooFitter.cpp

# All .o files rely on the .cpp files
%.o:%.cpp
	${CCC} ${ROO_INCLUDES} $< -o $@

# All .o files rely on the .h files
%.o:%.h
	${CCC} ${ROO_INCLUDES} $< -o $@

clean:
	rm RooFitter_cpp.d RooFitter_cpp.so
