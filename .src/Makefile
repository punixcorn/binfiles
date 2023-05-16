CC=gcc
CCP=g++
F1= -std=gnu2x
F2= -std=c2x 
D1= --all-warnings -g
D2=
FILE= 
args=
of=
target=ansi 

all : $(target)

ansi: obj.o
	$(CC) ${FILE} ${F2} ${D1} ${D2} -o cli
	

gnu: obj.o
	$(CC) ${FILE} ${F1} ${D1} ${D2} -o cli 


cpp: obj.o
	$(CCP) $(FILE) $(D1) -lfmt -o cli 

cpp.o: $(FILE)
	$(CCP) -c $(FILE) -lfmt -std=c++2a $(D1) $(D2)  -o obj.o


obj.o: ${FILE}
	$(CC) -c ${FILE} -std=c++2a $(D1) $(D2) -o obj.o

copy: cli 
	cp cli ${of}

run: cli 
	@echo "======================================================"
	@./cli

clean:
	@echo "cleaning..."
	@touch cli obj.o
	@rm cli obj.o

