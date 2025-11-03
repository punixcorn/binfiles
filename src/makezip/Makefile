all:
	cmake -S . -B build/
	cmake --build build/
	@echo
	@echo "Man file"
	#gzip makezip.1
	#sudo mv makezip.1.gz /usr/share/man/man1/

clean:
	rm -rf build/
	rm -rf bin/

debug:
	g++ main.cpp -lzip -lfmt -ggdb -o main -std=c++26 

run:
	@./bin/makezip 

