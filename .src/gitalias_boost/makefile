option=-h
DIR=

allmannual:
	g++ -std=c++20 Main.cxx -lfmt -lboost_system -lboost_execption -lboost_program_options -o main 

all: build

allClean: Main.cxx clean build

copy: all
	cp ./bin/gitalias $(DIR)

run: all
	./bin/gitalias $(option)

preBuild: Main.cxx
	cmake -S . -B build

build: preBuild
	cmake --build build 

clean:
	@rm -rf build
	@rm -rf bin
	@echo "Cleaning Done"
