CC=cmake
FILE=cstrap.cpp
main: build
	$(CC) --build build/

build: ${FILE}
	$(CC) -S . -B build/

run: main
	@echo "========================================="
	@./bin/testing

clean:
	@rm -rf bin build 2>/dev/null
