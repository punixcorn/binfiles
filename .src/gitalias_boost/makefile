option=-h
DIR=

all: build

allClean: preBuildClean

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
	rm -rf build
	rm -rf bin
