#include <assert.h>
#include <dirent.h>
#include <unistd.h>
#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
using std::string, std::string_view;

/*
 * [file] : runfile
 * [purpose] : compile and run a file
 * [status] : in development
 * [ currnet implemrntation ] : runfile [ filename ] || runfile { automatic :
 * main.c , main.cpp , main.py };
 * [sample] : runfile cpp || runfile main.cpp  || runfile < g++ main.cpp
 * -g -std=c2x -lfmt -o main
 */

/* do one thing and do it well
 * looks for a main.[c,cpp,rs,py] to run
 * you can specify by passing in the filename instead
 */

// a function will return this struct[ filename and if_found ]
struct Findmain {
    Findmain(string Name, bool Found) : filename(Name), found(Found){};
    string filename;
    bool found;
};

// pass in a struct
struct Trips {
   private:
    char** Args = nullptr;  // argv
    int argCount{};         // argc
    FILE* fptr = nullptr;
    string filename{};
    string command{};
    // options avaliable for each file

   public:
    Trips(char** argv, int argc) : Args(argv), argCount(argc){};
    ~Trips() {
        delete[] Args;
        delete fptr;
    };

    // any number(count) of arguments to put into command;
    void putsCommand(int count, ...);
    // set file name
    void setFilename(string Filename) { filename = Filename; };
    // run files
    void runcpp();
    void runpy();
    void runasm();
    void runc();
    void runrs();
    // find the file to run
    void friend setFile();
    // parse argc and argv
    void parse(void);
    // find main.* file
    Findmain friend* findmain(void);

};  // Trips

void Trips::putsCommand(int count, ...) {
    std::va_list args;
    va_start(args, count);
    assert(count != 0);
    while (count > 0) {
        command += string(va_arg(args, char*));
        count--;
    }
    va_end(args);
    assert(command.size() != 0);
}

void setFile(string filename) {
    assert(filename.size() != 0);
    Trips* t = new Trips(NULL, 0);
    t->setFilename(filename);
    switch (filename[filename.length() - 1]) {
        case 'p':  // .cpp
            t->runcpp();
            break;
        case 'c':  // .c
            t->runc();
            break;
        case 's':  // .rs
            t->runrs();
            break;
        case 'y':  //.py
            t->runpy();
            break;
        case 'm':  // yasm
            t->runasm();
            break;
        default:
            assert(false);
            break;
    }
};

void Trips::parse(void) {
    string name = program_invocation_name;
    assert(Args != NULL);
    switch (argCount) {
        case (2): {
            if (strcmp(Args[1], "-h") == 0 || strcmp(Args[1], "--help") == 0) {
                printf(
                    "%s : run a main.[c,cpp,py,rs] file in currnet dir \n"
                    "\n"
                    " -h                  print this message\n"
                    " %s [filename]       run the file(filename)\n"
                    " %s                  look for a main.[c,cpp,py,rs]\n"
                    "                     in current DIR and run it\n",
                    name.c_str(), name.c_str(), name.c_str());
                exit(0);
            } else if ((strstr(Args[1], ".py") != NULL) ||
                       (strstr(Args[1], ".cpp") != NULL) ||
                       (strstr(Args[1], ".nasm") != NULL) ||
                       (strstr(Args[1], ".c") != NULL) ||
                       (strstr(Args[1], ".rs") != NULL)) {
                setFile(Args[1]);
                return;
            } else {
                printf(
                    "ERR: invalid file passed\nTry %s -h for more info "
                    "called\n",
                    name.c_str());
                exit(1);
            }
            break;
        }  // case 2
        default: {
            printf("ERR: invalid file passed\nTry %s -h for more info called\n",
                   name.c_str());
            exit(1);
        }
    }
};

void Trips::runcpp() {
    printf("compiling %s into outputfile : main\n", filename.c_str());
    putsCommand(3, "g++ ", filename.c_str(),
                " -lfmt -g -std=c++2a --all-warnings -o main ");
    system(command.c_str());
    exit(0);
};
void Trips::runpy() {
    printf("running %s\n", filename.c_str());
    string command = "python3 " + filename;
    system(command.c_str());

    exit(0);
};
void Trips::runasm() {
    printf("running %s using Fasm\n", filename.c_str());
    string command{"fasm " + filename};

    exit(0);
};

void Trips::runc() {
    printf("compiling %s into outputfile : main\n", filename.c_str());
    string command = "gcc " + filename + " --all-warnings -g -std=c2x -o main ";
    system(command.c_str());
    exit(0);
};
void Trips::runrs() {
    printf("compiling %s into outputfile : main\n", filename.c_str());
    string command = "rustc " + filename + " -o main ";
    system(command.c_str());
    exit(0);
};

// main.py, rs , cpp ,  c, asm
// find main.[something]
[[nodiscard("name of file, if found")]] Findmain* findmain() {
    string file{}, place{"."};
    DIR* d{};
    dirent* dir{};
    d = opendir(place.c_str());
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            file = dir->d_name;
            if (file == "main.c") {
                return new Findmain("main.c", true);
                closedir(d);
            } else if (file == "main.cpp") {
                return new Findmain("main.cpp", true);
                closedir(d);
            } else if (file == "main.py") {
                return new Findmain("main.py", true);
                closedir(d);
            } else if (file == "main.rs") {
                return new Findmain("main.rs", true);
                closedir(d);
            }
        }
    }
    closedir(d);
    return new Findmain("", false);
}

int main(int argc, char** argv) {
    if (argc == 1) {
        Findmain* found;
        found = findmain();
        if (!found->found) {
            fprintf(stderr, "Err:Could not find the a main.* file to run\n");
        } else {
            setFile(found->filename);
        }
        delete found;
        exit(0);
    }
    if (argc >= 2) {
        Trips* t = new (std::nothrow) Trips(argv, argc);
        t->parse();
        return 0;
    }
}
