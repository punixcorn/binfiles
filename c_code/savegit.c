// no files linked had to do this from sratch
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char temp[100], message[500], command[1024];
int i = 1, sflag = 0;

void reset(int num);
void input(char *arg);
void sort(char *str);
void savegit();
int main(int argc, char **argv) {
  if (argc > 1) {
    goto argc2;
  }
  printf("incorrect arguments passed\n***\tprogram terminated\n");
  exit(0);
argc2:
  input("cd $(pwd) && git add . && git commit -m '");
  while (i <= argc) {
    int length = strlen(*(argv + i));
    if (length > 2 && *(argv + i)[0] != '-') {
      strcat(message, *(argv + i));
      break;
    };
    i++;
  }

  reset(1);
  while (i < argc) {
    sort(*(argv + i));
    i++;
  }
  savegit();
  exit(0);
}

void input(char *arg) { strcat(command, arg); }

void sort(char *str) {
  if (str[0] == '-') {
    switch (*(str + 1)) {
    case 'm':
      input(message);
      input("'");
      break;
    case 'y':
      printf("saving and pushing \n");
      strcat(temp, " && git push");
      break;
    case 'n':
      printf("saving and no pushing\n");
      break;
    default:
      printf("saving and no pushing\n");
      break;
    };
  }
}

void savegit() {
  input(temp);
  system(command);
}

void reset(int num) {
  *&i = 0;
  *&i += num;
}
