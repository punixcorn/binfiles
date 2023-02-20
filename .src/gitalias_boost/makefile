option=-h

all: build

run: all 
	./bin/gitalias $(option)

preBuild: Main.cxx clean
	cmake -S . -B build

build: preBuild
	cmake --build build

clean:
	rm -rf build
	rm -rf bin
