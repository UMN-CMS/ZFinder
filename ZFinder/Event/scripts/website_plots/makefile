# Pull in ROOT
ROOT_INCLUDES=`root-config --cflags`
ROOT_ALL=`root-config --cflags --libs`

# Compiler
CXX=g++
CXXFLAGS=-O3 -g -std=c++0x -Wall -pipe -pedantic -Wextra

# Command to remove files
RM=rm -f

# Make a list of all the files we need
# We use substitution to make the other lists from the cpp list
CPP_FILES := $(wildcard plotters/*.cpp) # Make a list of all cpp files
EXE_FILES := $(subst .cpp,.exe,$(CPP_FILES)) # Replace '.cpp' with '.exe'
OBJ_FILES := plotters/style.o

# Make all the files in $(EXE_FILES)
all: $(EXE_FILES)

# Every .exe file is derived from the .cpp with the same name
plotters/%.exe: plotters/%.cpp $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(ROOT_ALL) -o $@ $^

# The style file common to all plotters
plotters/style.o: plotters/lib/style.h plotters/lib/style.cpp
	$(CXX) $(CXXFLAGS) $(ROOT_ALL) -c plotters/lib/style.cpp -o $@

# Clean up
clean:
	$(RM) $(OBJ_FILES)

clean-all:
	$(RM) $(EXE_FILES) $(OBJ_FILES)
