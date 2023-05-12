#include <assert.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
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
 * [sample] : runfile cpp -p -lfmt
 *            < g++ main.cpp -g -std=c2x [-lfmt ...] -o main && ./main
 *
 * */

/* to set a default file
 * write into a persistence place
 * Using Classes here is a bad idea,
 * Design will be bad
 */

union Format {
  enum asmfmt { fasm, nasm, gnu, yasm };
  enum cppfmt { ISO, GNU };
};

union c {
  std::string std;
  std::string debug;
};
union rs {};
union py {};
union nasm {};

struct others {
  others(){};
  std::string extras;
  c cc;
  py pp;
  rs rr;
  nasm na;
};

// pass in a struct  -> something ->
struct Trips {
private:
  char **Args = nullptr; // argv
  int argCount{};        // argc
  FILE *fptr = nullptr;
  std::string filename{};
  // options avaliable for each file
  others *otr;

public:
  Trips(char **argv, int argc, others *Ot)
      : Args(argv), argCount(argc), otr(Ot){};
  ~Trips() {
    delete[] Args;
    delete fptr;
  };

  // run files
  void runcpp();
  void runpy();
  void runasm(Format::asmfmt fmt);
  void runc();
  void runrs();
  // find the file to run
  void setFile();
  void parse(void);
  bool findmain(void);

}; // Trips

void Trips::setFile() {
  assert(filename.size() != 0);
  switch (filename[filename.length() - 1]) {
  case 'p': // .cpp
    runcpp();
    break;
  case 'c': // .c
    runc();
    break;
  case 's': // .rs
    runrs();
    break;
  case 'y': //.py
    runpy();
    break;
  case 'm': // nasm
    runasm(Format::asmfmt::nasm);
    break;
  default:
    assert(false);
    break;
  }
};

void Trips::parse(void) {
  assert(Args != NULL);
  switch (argCount) {
  case (2): {
    if (strcmp(Args[1], "-h") == 0) {
      fmt::print("help called\n");
      exit(0);
    } else if ((strstr(Args[1], ".py") != NULL) ||
               (strstr(Args[1], ".cpp") != NULL) ||
               (strstr(Args[1], ".nasm") != NULL) ||
               (strstr(Args[1], ".c") != NULL) ||
               (strstr(Args[1], ".rs") != NULL)) {
      fmt::print("file found : {}\n", Args[1]);
      filename = Args[1];
      setFile();
      return;
    } else {
      fmt::print("ERR: invalid file passed\nTry {} -h for more info called\n",
                 program_invocation_name);
      exit(1);
    }
    break;
  } // case 2
  default: {
    // for now the only option is the other formats
    //  -l -f -v
    //  only c / cpp has these things [ i dont know about the other languages
    //  use -p ]
    int temp{1};
    while (temp < argCount) {
      if (strncmp(Args[temp], "-l", 2) == 0) {
        // parse -l
        temp += 1;
        while (Args[temp][0] != '-') {
          if (Args[temp] != NULL)
            otr->extras += Args[temp];
          temp++;
        }
      };
      if (strncmp(Args[temp], "-p", 2) == 0) {
        // parse -p
        temp += 1;
      }
      if (strncmp(Args[temp], "-s", 2) == 0) {
        temp += 1;
        if (Args[temp] != NULL)
          otr->cc.std = "asd";

        // -f
      }

      temp++;
    }
    break;
  }
  }
};

void Trips::runcpp(){};
void Trips::runpy(){};
void Trips::runasm(Format::asmfmt fmt){};
void Trips::runc(){};
void Trips::runrs(){};

// main.py, rs , cpp ,  c, asm
// find main.[something]
[[nodiscard("it returns number of main.* found or a string ")]] bool
Trips::findmain() {
  std::string file{}, thing{}, place{};
  short count{};
  DIR *d{};
  dirent *dir{};
  d = opendir(place.c_str());
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      file = dir->d_name;
      if (file == "main.c") {
        count += 1;
      }
      closedir(d);
    }
  }
  return 1;
}

int main(int argc, char **argv) {
  assert(argc >= 1);
  others ot;
  Trips *t = new (std::nothrow) Trips(argv, argc, &ot);

  t->parse();
  return 0;
}
