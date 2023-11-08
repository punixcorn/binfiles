CC=gcc
CC_=g++
CFLAGS= -std=gnu2x --all-warnings -ggdb
CPPFLAGS= -std=gnu++2b --all-warnings -ggdb 
FILE=
DFLAGS=
OUTPUT=cli

c: cmain.o 
	$(CC) $(FILE) -o $(OUTPUT) $(CFLAGS) $(DFLAGS)

cmain.o: $(FILE)
	$(CC) -c $(FILE) $(CFLAGS) -o cmain.o $(DFLAGS)

cpp:cppmain.o
	$(CC_) $(FILE) -o $(OUTPUT) $(CPPFLAGS) $(DFLAGS)

cppmain.o: $(FILE)
	$(CC_) -c $(FILE) $(CPPFLAGS) -o cmain.o  $(DFLAGS)

run:
	@echo"=========================================="
	@./$(OUTPUT) 

clean:
	@echo "cleaning..."
	@rm *.o 
	@rm $(OUTPUT)
