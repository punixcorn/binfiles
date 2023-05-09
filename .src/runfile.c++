#include <cstdlib>
#include <fmt/format.h>

/*
 * [file] : runfile
 * [purpose] : compile and run a file
 * [status] : in development
 * [sample] : runfile cpp -p -lfmt
 *            < g++ main.cpp -g -std=c2x [-lfmt ...] -o main && ./main
 * */
typedef enum { fasm, nasm, gnu, yasm } asmfmt;

struct Trips {};
void parse(int);
void runcpp(char *file);
void runpy(char *file);
void runasm(char *file, asmfmt fmt);
void runc(char *file);
void runrs(char *file);

int main(int argc, char **argv) {
  if (argc <= 1) {
    fprintf(stderr, "%s : invalid arguments\n", *argv);
    exit(1);
  }

  exit(0);
}

void runcpp(char *file){};
void runpy(char *file){};
void runasm(char *file, asmfmt fmt){};
void runc(char *file){};
void runrs(char *file){};
