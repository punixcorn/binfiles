CC=g++
option=-h
DIR=
FLAGS= -static -g -std=c++17 -O3 -Walloca -Walloc-zero -Warray-bounds -Wbool-compare -Wconversion-null -Wformat -Wformat-overflow -Wunused-variable -Wuseless-cast -Wvector-operation-performance --debug
BOOST_FLAGS= -lboost_program_options -lboost_exception -lboost_system

# these will run cmake which the libs are dynamic
all: build

allClean: preBuildClean

allStaticCopy:allStatic
	cp ./gitalias_Static ../../gitalias

allStatic: Main.o
	$(CC) -o"gitalias_Static" Main.o $(BOOST_FLAGS) $(FLAGS)
	
Main.o:
	$(CC) -c Main.cxx

# to make a copy somewhere for testing
copy: all
	cp ./bin/gitalias $(DIR)

run: all
	./bin/gitalias $(option)

preBuildClean: Main.cxx clean preBuild build

preBuild: Main.cxx
	cmake -S . -B build

build: preBuild
	cmake --build build 

clean:
	@touch gitalias_Static Main.o
	@rm Main.o gitalias_Static
	@rm -rf build
	@rm -rf bin
