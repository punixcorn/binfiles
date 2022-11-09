// savegit -m "hey there" -y
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char temp[100], message[500], command[1024];
int i = 1, sflag = 0;

void tripFlag();
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
    } else if (length > 2 && *(argv + i)[0] == '-') {
      ;
    }
    i++;
  }
  /* debug statement*/
  printf("%s : should be the message\n", message);

  reset(1);
  /* debug statement*/
  printf("%d : should be 1\n", i);
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
  /*debug statement*/
  printf("%s:should be the command\n", command);
  system(command);
}

void reset(int num) {
  *&i = 0;
  *&i += num;
}

void tripFlag() { *&sflag = 1; }
