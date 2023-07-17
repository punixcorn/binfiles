#include <assert.h>
#include <cctype>
#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

using std::string, std::stringstream;

/* [ name ]: makehere
 * [ purpose ] : create a simple c/c++ project [ status ] : complete
 * [ sample ] : makehere -l [cpp/c] -C [make/cmake] -std [11/14/17/20/2a/2x] -f
 * <filename>
 */

// engine will take the argc and argv and parse them.
// parse => check flags and call the respective functions

struct Engine {
   private:
    char** Args;
    int Argc;
    bool skipfile{false};
    string language;
    string filename;
    string compiler;
    string standard;

   public:
    Engine(char** args, int argc) : Args(args), Argc(argc){};
    // functions to use
    void ERR(string str, int status = 1) {
        fprintf(stderr, "%s : [ERR] %s\n", program_invocation_name,
                str.c_str());
        exit(status);
    }  // ERR

    // compares all va_args to str and returns true if one is fouund
    bool compareStrings(const char* str, int count, ...) {
        va_list list;
        va_start(list, count);
        if (!count)
            return false;
        while (count > 0) {
            if ((strcmp(str, va_arg(list, char*)) == 0)) {
                return true;
            }
            count--;
        }
        return false;
    };

    // checks if the str has a '-' in it
    void check_next(const char* str, const char* err_message) {
        if (str != NULL) {
            if (str[0] != '-') {
                return;
            }
        }
        string err_ = string(err_message);
        ERR(err_);
    }  // check_next

    // creates a file and returns a pointer to it
    FILE* getFile(const char* file) {
        FILE* tempfile = fopen(file, "w+");
        assert(tempfile != NULL);
        return tempfile;
    };  // geFile

    // create a file and puts in the strings into the file
    void createFile(const char* str, const char* nameOfFile, int free = 1) {
        assert(str != NULL);
        FILE* makefile = getFile(nameOfFile);
        fwrite(str, 1, strlen(str), makefile);
        fclose(makefile);
        if (free)
            delete[] str;
    };  // createFile

    void clangd() {
        char* str = new char[500];
        sprintf(str,
                "CompileFlags:\n"
                "\tAdd: [ -std=%s ]\n",
                (language == "c" ? "gnu2x" : "gnu++2a"));

        createFile(str, ".clangd");
    };  // clangd

    void make() {
        char* str = new char[500];
        sprintf(str,
                "CC=%3s\n"
                "FILE=%s\n"
                "STD=-std=%s%s\n"
                "FLAGS=-g --all-warnings\n"
                "main: main.o\n"
                "\t$(CC) main.o ${STD} ${FLAGS} -o main\n\n"
                "main.o: ${FILE}\n"
                "\t$(CC) -c ${FILE} ${STD} ${FLAGS} -o main.o\n\n"
                "run: main\n"
                "\t@echo \"=========================================\"\n"
                "\t@./main\n\n"
                "clean:\n"
                "\t@touch main.o main\n"
                "\t@rm main.o main\n",
                (language == "c" ? "gcc" : "g++"), filename.c_str(),
                (language == "c" ? "c" : "c++"), standard.c_str());
        createFile(str, "Makefile");
    };  // Make

    void cmake() {
        char* str = new char[500];
        if (language == "c") {
            language = "C";
        } else {
            language = "CXX";
        }
        sprintf(str,
                "cmake_minimum_required(VERSION 3.1)\n"
                "project(main)\n"
                "add_executable(${PROJECT_NAME} %s)\n"
                "set(CMAKE_%s_STANDARD %s)\n"
                "set(CMAKE_%s_STANDARD_REQUIRED true)\n"
                "set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)\n",
                filename.c_str(), language.c_str(), standard.c_str(),
                language.c_str());
        createFile(str, "CMakeLists.txt");
        char* makestr = new char[500];
        memset(makestr, 0, strlen(str));
        sprintf(makestr,
                "CC=cmake\n"
                "FILE=%s\n"
                "main: build\n"
                "\t$(CC) --build build/\n\n"
                "build: ${FILE}\n"
                "\t$(CC) -S . -B build/\n\n"
                "run: main\n"
                "\t@echo \"=========================================\"\n"
                "\t@./bin/main\n\n"
                "clean:\n"
                "\t@rm -rf bin build 2>/dev/null\n",
                filename.c_str());
        createFile(makestr, "Makefile");
    }  // Cmake

    // parser -l -c -h -std -f -s
    void parse() {
        switch (Argc) {
            case 2:
                if (compareStrings(Args[1], 2, "--help", "-h")) {
                    printf(
                        "%s : create a simple project startup \n"
                        "-h    --help                       output this "
                        "message\n"
                        "-l    --language   [c,cpp]         language of the "
                        "project*\n"
                        "-c    --complier   [cmake,make]    compiler Script  ( "
                        "default: "
                        "make )\n"
                        "-f    --filename   [filename]      custom file name ( "
                        "default: "
                        "main.[c,cpp] )\n"
                        "-std  --standard   [standard]      set a standard   ( "
                        "default: "
                        "-std=c2- )\n"
                        "-s    --skip                       skip creating a "
                        "source "
                        "file\n"
                        "NB: Only -l is compulsory\n",
                        program_invocation_name);

                    exit(0);
                } else {
                    ERR("Invalid arguments passed", 1);
                }
                break;
            default:
                int i{1};
                while (i < Argc) {
                    if (compareStrings(Args[i], 2, "--language", "-l") == 1) {
                        ++i;
                        check_next(Args[i],
                                   "-l / --language needs an argument");
                        language = string(Args[i]);
                        if ((language != "c") && (language != "cpp")) {
                            ERR("Yo man, only c or cpp allowed in "
                                "-l/--language",
                                1);
                        }
                    } else if (compareStrings(Args[i], 2, "--filename", "-f")) {
                        ++i;
                        check_next(Args[i], "-f needs an argument");
                        filename = string(Args[i]);
                    } else if (compareStrings(Args[i], 2, "--compiler", "-c")) {
                        ++i;
                        check_next(Args[i], "-c needs an argument");
                        compiler = string(Args[i]);
                    } else if (compareStrings(Args[i], 2, "-std",
                                              "--standard")) {
                        ++i;
                        check_next(Args[i], "-std needs an argument");
                        standard = string(Args[i]);
                    } else if (compareStrings(Args[i], 2, "-s", "--skip")) {
                        skipfile = true;
                    }
                    i++;
                };
                break;
        };

        if (language.size() == 0) {
            ERR("-l is compulsory , needed to determine the compiler script "
                "and the "
                "filename",
                1);
        }
        // checking standard
        if (standard != "11" || standard != "17" || standard != "20" ||
            standard != "99" || standard != "2x" || standard != "2a") {
            if (language == "c") {
                standard = "2x";
            } else {
                standard = "2a";
            }
        }
        // setting the filename
        if (filename.size() == 0) {
            if (language == "c") {
                filename = "main.c";
            } else {
                filename = "main.cpp";
            }
        }

        // creating the file
        // checking if  c lang is used with a cpp file
        if ((language == "c") && (filename.find(".c") == string::npos)) {
            fprintf(stderr,
                    "Warning: language selected is C and Filename is not a C "
                    "file\n");
        } else if ((language == "cpp") &&
                   (filename.find(".cpp") == string::npos)) {
            fprintf(stderr,
                    "Warning: language selected is C++ and Filename is not a "
                    "C++ file\n");
        }
        string start =
            "#include<stdio.h>\nint main(int argc,char ** argv){\n}\n";
        if (!skipfile)
            createFile(start.c_str(), filename.c_str());
        // creating the compiler script
        if (compiler.size() == 0) {
            compiler = "make";
        }
        if (compiler == "make") {
            make();
        } else if (compiler == "cmake") {
            cmake();
        }

        // setting up clangd
        clangd();

        printf(
            "Created a project\n"
            "language        : %s\n"
            "filename        : %s\n"
            "compiler script : %s\n"
            "standard        : %s\n",
            language.c_str(), skipfile ? "--skipped--" : filename.c_str(),
            compiler.c_str(), standard.c_str());
        exit(0);
    }  // void Parse

};  // Engine

int main(int argc, char** argv) {
    Engine e(argv, argc);
    if (argc == 1) {
        e.ERR("no arguments passed\ntry -h for more info\n", 1);
    }
    e.parse();
    return 0;
}
