<<<COMMENT
FILES=zip.cpp Makezip.cpp main.cpp
CC=g++
LIBS=-lboost_program_options -lzip
IO= newzip.zip

all: $(FILES)
	$(CC) $(FILES) -std=c++26 $(LIBS) -o main -ggdb

run:
	./main $(IO)

clean:
	rm main
COMMENT
