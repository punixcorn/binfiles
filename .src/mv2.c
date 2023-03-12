#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* file name : mv2
 * file purpose: create a directory if it doesnt exist when moving a file
 * mode : stable , not fully tested , debug codes still avaliable
 * sample : mv2 --mkdir alacritty.yml ~/.config/alacritty/
 */

char *pname = "mv2";
void info(int stat) {
  if (stat) {
    fprintf(stderr, "%s: Error Occured\n", pname);
    exit(stat);
  }
  fprintf(stdout,
          "%s: mv command but with extra args...\n"
          "Usage: %s [ options... ] [ file... ] [ place... ]\n"
          "Options:\n"
          "\t--mkdir        create the directory if it doesnt exist\n",
          pname, pname);
  exit(stat);
}

void createdir(char *dname, mode_t mode) {
  DIR *checkdir = opendir(dname);
  int check = 0;
  if (checkdir == NULL)
    check = mkdir(dname, mode) == 0 ? 0 : 1;
  if (check)
    info(check);
}

int main(int argc, char **argv) {
#if dargc
  printf("this is argc :%d\n", argc);
#endif

  char *pdir = (char *)malloc(sizeof(char) * 100);
  int check = 0, i = 1;
  char command[600];

  if (argc < 2 && (pdir == NULL)) {
    info(1);
  }
  if (argc == 2) {
    if (strcmp(*(argv + 1), "-h") == 0)
      info(0);
    else
      info(1);
  }

  sprintf(command, "mv ");
  while (i < argc) {
    if (strcmp(*(argv + i), "--mkdir") == 0) {
      check = 1;
      ++i;
    }
    strcat(command, " ");
    strcat(command, *(argv + i));
    i++;
  }

#if dcommand
  printf("Command : %s\nCheck is %d\n", command, check);
  exit(0);
#endif
  if (check)
    createdir(*(argv + (argc - 1)), 0710);

  system(command);
  exit(0);
}
