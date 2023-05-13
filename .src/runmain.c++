#include <assert.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <unistd.h>

/*
 * [file] : runfile
 * [purpose] : compile and run a file
 * [status] : in development
 * [ currnet implemrntation ] : runfile [ filename ] || runfile { automatic :
 * main.c , main.cpp , main.py };
 * [sample] : runfile cpp -p -lfmt < g++ main.cpp
 * -g -std=c2x [-lfmt ...] -o main && ./main
 */

/* to set a default file
 * write into a persistence place
 * Using Classes here is a bad idea,
 * Design will be bad
 */

union Format
{
    enum asmfmt
    {
        fasm,
        nasm,
        gnu,
        yasm
    };
    enum cppfmt
    {
        ISO,
        GNU
    };
};

struct Findmain
{
    Findmain(std::string Name, bool Found)
      : filename(Name)
      , found(Found){};
    std::string filename;
    bool found;
};

// pass in a struct  -> something ->
struct Trips
{
private:
    char** Args = nullptr; // argv
    int argCount{};        // argc
    FILE* fptr = nullptr;
    std::string filename{};
    // options avaliable for each file

public:
    Trips(char** argv, int argc)
      : Args(argv)
      , argCount(argc){};
    ~Trips()
    {
        delete[] Args;
        delete fptr;
    };

    // set file name
    void setFilename(std::string Filename) { filename = Filename; };
    // run files
    void runcpp();
    void runpy();
    void runasm(Format::asmfmt fmt);
    void runc();
    void runrs();
    // find the file to run
    void friend setFile();
    void parse(void);
    Findmain friend* findmain(void);

}; // Trips

void
setFile(std::string filename)
{
    assert(filename.size() != 0);
    Trips* t = new Trips(NULL, 0);
    t->setFilename(filename);
    switch (filename[filename.length() - 1]) {
        case 'p': // .cpp
            t->runcpp();
            break;
        case 'c': // .c
            t->runc();
            break;
        case 's': // .rs
            t->runrs();
            break;
        case 'y': //.py
            t->runpy();
            break;
        case 'm': // nasm
            t->runasm(Format::asmfmt::nasm);
            break;
        default:
            assert(false);
            break;
    }
};

void
Trips::parse(void)
{
    assert(Args != NULL);
    switch (argCount) {
        case (2): {
            if (strcmp(Args[1], "-h") == 0 || strcmp(Args[1], "--help") == 0) {
                std::string_view name = program_invocation_name;
                fmt::print(
                  "{} : run a main.[c,cpp,py,rs] file in currnet dir \n"
                  "\n"
                  " -h                  print this message\n"
                  " {} [filename]       run the file(filename)\n"
                  " {}                  look for a main.[c,cpp,py,rs]\n"
                  "                     in current DIR and run it\n",
                  name,
                  name,
                  name);
                exit(0);
            } else if ((strstr(Args[1], ".py") != NULL) ||
                       (strstr(Args[1], ".cpp") != NULL) ||
                       (strstr(Args[1], ".nasm") != NULL) ||
                       (strstr(Args[1], ".c") != NULL) ||
                       (strstr(Args[1], ".rs") != NULL)) {
                fmt::print("file found : {}\n", Args[1]);
                setFile(Args[1]);
                return;
            } else {
                fmt::print(
                  "ERR: invalid file passed\nTry {} -h for more info called\n",
                  program_invocation_name);
                exit(1);
            }
            break;
        } // case 2
        default: {
            // for now the only option is the other formats
            //  -l -f -v
            //  only c / cpp has these things [ i dont know about the other
            //  languages use -p ]
            int temp{ 1 };
            while (temp < argCount) {
                if (strncmp(Args[temp], "-l", 2) == 0) {
                    // parse -l
                    temp += 1;
                    while (Args[temp][0] != '-') {
                        if (Args[temp] != NULL) temp++;
                    }
                };
                if (strncmp(Args[temp], "-p", 2) == 0) {
                    // parse -p
                    temp += 1;
                }
                if (strncmp(Args[temp], "-s", 2) == 0) {
                    temp += 1;
                    if (Args[temp] != NULL)

                        // -fl=
                        ;
                };

                temp++;
            }
            break;
        }
    }
};

void
Trips::runcpp()
{
    fmt::print("compiling {} into outputfile : main\n", filename);
    std::string command =
      "g++ " + filename + " -lfmt -g -std=c++2a --all-warnings -o main ";
    system(command.c_str());
    exit(0);
};
void
Trips::runpy()
{
    fmt::print("running {}\n", filename);
    std::string command = "python3 " + filename;
    system(command.c_str());

    exit(0);
};
void
Trips::runasm(Format::asmfmt fmt)
{
    system("echo\" Asm aka nasm is not ready yet \"");
    exit(0);
};

void
Trips::runc()
{
    fmt::print("compiling {} into outputfile : main\n", filename);
    std::string command =
      "gcc " + filename + " --all-warnings -g -std=c2x -o main ";
    system(command.c_str());
    exit(0);
};
void
Trips::runrs()
{
    fmt::print("compiling {} into outputfile : main\n", filename);
    std::string command = "rustc " + filename + " -o main ";
    system(command.c_str());
    exit(0);
};

// main.py, rs , cpp ,  c, asm
// find main.[something]
[[nodiscard("it returns a struct { name_of_file:std::string , found:bool "
            "}")]] Findmain*
findmain()
{
    std::string file{}, place{ "." };
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

int
main(int argc, char** argv)
{
    if (argc == 1) {
        Findmain* found;
        found = findmain();
        if (found->found == false) {
            fmt::print(stderr, "Err:Could not find the a main.* file to run\n");
            delete found;
            exit(1);
        } else {
            setFile(found->filename);
            exit(0);
        }
    }
    if (argc >= 2) {
        Trips* t = new (std::nothrow) Trips(argv, argc);
        t->parse();
        return 0;
    }
}
