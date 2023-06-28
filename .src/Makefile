CC=gcc
CC_=g++
CFLAGS= -std=gnu2x --all-warnings -g -ggdb
CPPFLAGS= -std=gnu++2b --all-warnings -ggdb 
FILE=
DFLAGS=

c: cmain.o 
	$(CC) $(FILE) -o cli $(CFLAGS) $(DFLAGS)

cmain.o: $(FILE)
	$(CC) -c $(FILE) $(CFLAGS) -o cmain.o $(DFLAGS)

cpp:cppmain.o
	$(CC_) $(FILE) -o cli $(CPPFLAGS) $(DFLAGS)

cppmain.o: $(FILE)
	$(CC_) -c $(FILE) $(CPPFLAGS) -o cmain.o  $(DFLAGS)

run:
	@echo"=========================================="
	@./cli 

clean:
	@echo "cleaning..."
	@rm *.o 
	@rm cli
