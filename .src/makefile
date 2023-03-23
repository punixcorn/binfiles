CC=gcc
F1= -std=gnu2x
F2= -std=c2x 
D1= --all-warnings -g
D2=
FILE= 
args=
of=

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

copy: cli 
	cp cli ../${of}
clean:
	@echo "cleaning..."
	@touch cli obj.o
	@rm cli obj.o

