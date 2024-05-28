all:
	cmake -S . -B build/
	cmake --build build/

clean:
	rm -rf build/
	rm -rf bin/

debug:
	g++ main.cpp Makezip.cpp zip.cpp errormessage.cpp -lboost_program_options -lboost_system -lboost_exception -lzip -ggdb -o main -std=c++26

run:
	@./bin/makezip 

