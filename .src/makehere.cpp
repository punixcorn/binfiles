#include <assert.h>
#include <cctype>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fmt/core.h>
#include <fmt/format.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
using std::string, std::stringstream;

/* [ name ]: setProgect -l [cpp/c] -C [make/cmake] -std [11/14/17/20/2a/2x]
 * [ purpose ] : create a simple c/c++ project
 * [ status ] : ready
 * [ sample ] : setProgect -l [cpp/c] -C [make/cmake] -std [11/14/17/20/2a/2x]
 */

// engine will take the argc and argv and parse them ?? into what ?
// parse them and call the respective functions

struct Engine {
private:
  char **Args;
  int Argc;
  string language;
  string filename;
  string compiler;
  string standard;

public:
  Engine(char **args, int argc) : Args(args), Argc(argc){};
  // functions to use
  void ERR(std::string str, int status = 1) {
    fmt::print(stderr, "{} : [ERR] {}\n", program_invocation_name, str);
    exit(status);
  }
  FILE *getFile(const char *file) {
    FILE *tempfile = fopen(file, "w+");
    assert(tempfile != NULL);
    return tempfile;
  };

  void createFile(const char *str, const char *nameOfFile) {
    assert(str != NULL);
    FILE *makefile = getFile(nameOfFile);
    fwrite(str, 1, strlen(str), makefile);
    fclose(makefile);
    delete[] str;
  };

  void make() {
    char *str = new char[500];
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
  };

  void cmake() {
    char *str = new char[500];
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
    char *makestr = new char[500];
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
  }

  // parser -l -c -h
  void parse() {
    switch (Argc) {
    case 2:
      if ((strcmp(Args[1], "-h") == 0) || (strcmp(Args[1], "--help") == 0)) {
        printf(
            "%s : create a simple project startup \n"
            "-h                 output this message\n"
            "-l [c/cpp]         language of the project*\n"
            "-c [cmake,make]    compiler Script  ( default: make )\n"
            "-f [filename]      custom file name ( default: main.[c | cpp] )\n"
            "-std [standard]    set a standard   ( default: -std=[c2x | "
            "c++2a] )\n"
            "\nOnly -l is compulsory\n",
            program_invocation_name);

        exit(0);
      } else {
        ERR("Invalid arguments passed", 1);
      }
      break;
    default:
      int i{1};
      while (i < Argc) {
        if (strcmp(Args[i], "-l") == 0) {
          ++i;
          assert(Args[i] != NULL);
          language = string(Args[i]);
          if ((language != "c") && (language != "cpp")) {
            ERR("Yo man, only c or cpp allowed in -l", 1);
          }
        } else if (strcmp(Args[i], "-f") == 0) {
          ++i;
          assert(Args[i] != NULL);
          filename = string(Args[i]);
        } else if (strcmp(Args[i], "-c") == 0) {
          ++i;
          assert(Args[i] != NULL);
          compiler = string(Args[i]);
        } else if (strcmp(Args[i], "-std") == 0) {
          ++i;
          assert(Args[i] != NULL);
          standard = string(Args[i]);
        }
        i++;
      };
      break;
    };

    if (language.size() == 0) {
      ERR("-l is compulsory , needed to determine the compiler script and the "
          "filename",
          1);
    }
    // checking standard
    if (standard != "11" || standard != "14" || standard != "17" ||
        standard != "20" || standard != "99" || standard != "2x" ||
        standard != "2a") {
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
              "Warning: language selected is C and Filename is not a C file\n");
    } else if ((language == "cpp") && (filename.find(".cpp") == string::npos)) {
      fprintf(
          stderr,
          "Warning: language selected is C++ and Filename is not a C++ file\n");
    }
    string start = "#include<stdio.h>\nint main(int argc,char ** argv){\n}\n";
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
    printf("Created a project main\n"
           "language        : %s\n"
           "filename        : %s\n"
           "compiler script : %s\n"
           "standard        : %s\n",
           language.c_str(), filename.c_str(), compiler.c_str(),
           standard.c_str());
    exit(0);
  }
};

int main(int argc, char **argv) {
  Engine *e = new Engine(argv, argc);
  if (argc == 1) {
    e->ERR("no arguments passed\ntry -h for more info\n", 1);
  }
  e->parse();
  return 0;
}
