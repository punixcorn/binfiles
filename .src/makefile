CC=gcc
F1= -std=gnu99
F2= -ansi 
D1= --all-warnings -g
D2=
FILE=test.c #move file into test.c, then do edits, later move back
args=

run: all-ansi 
	@echo "==========================RUNNING===========================" && [ -f ./cli ] && ./cli ${args} 

run-debug: debug
	@echo "==========================RUNNING===========================" && [ -f ./cli ] && ./cli ${args} 

all-ansi: obj.o
	$(CC) ${FILE} ${F2} -o cli

all-gnu: obj.o
	$(CC) ${FILE} ${F1} -o cli 

debug-gnu: obj.o
	$(CC) ${FILE} ${F1} ${D1} ${D2} -o cli 

debug-ansi: obj.o
	$(CC) $(FILE) ${F2} ${D1} ${D2} -o cli 

obj.o: ${FILE}
	$(CC) -c ${FILE} -o obj.o

clean:
	@touch cli obj.o
	@rm cli obj.o

