#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char command[100];
int main(int argc, char **argv) {

  if (argc == 2) {
    if (*(*(argv + 1)) == '-' && *(*(argv + 1) + 1) == 'h') {
      fprintf(stdout,
              "Usage : %s [ options... ]\n"
              "<directory>  mount this directory\n"
              "-h           print this message\n",
              *(argv));
      exit(0);
    }
    goto argc2;
  }

  fprintf(stderr,
          "%s : error, invaild arguments passed...\n"
          "Try %s -h for help\n",
          *(argv), *(argv));
  exit(1);
argc2:
  sprintf(command, " sudo umount %s", *(argv + 1)); // not
  system(command);
  exit(0);
}
