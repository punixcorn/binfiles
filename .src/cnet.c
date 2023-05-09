#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* [ file name ] : cnet
 * [ file purpose ] : ping archlinux.org aka check network
 * [ mode ] : stable
 * [ sample ] : cnet
 * [ sample2 ]: cnet 4
 */

char command[100];
void Error(char *name) {
  fprintf(stderr,
          "ERR : Invaild values passed\n"
          "Usage : %s [ number of pings ]\n",
          name);
  exit(1);
}
int main(int argc, char **argv) {
  sprintf(command, "ping archlinux.org");
#if DEBUG
  printf("this argc = %d\n", argc);
#endif
  if (argc == 1) {
    system(command);
    exit(0);
  }
  if (argc > 2)
    Error(*(argv));
  if (*(*(argv + 1)) == '-' && *(*(argv + 1) + 1) == 'h') {
    fprintf(stdout, "Usage : %s [ number of pings ]\n", *(argv));
    exit(0);
  }
  if (atoi(*(argv + 1)) == 0) {
    Error(*(argv));
  }
  strcat(command, " -c");
  strcat(command, *(argv + 1));
  system(command);
  exit(0);
}
