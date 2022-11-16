#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char temp[100], command[1024], message[500], push[30];
int t_int = 0, i = 1;

typedef struct {
  _Bool iTrip, yTrip, mTrip;
} Trip;
Trip trip;

void initTrip(Trip *T_trip);
void input(char *str);
void reset(int num);
void sort(char *str2);
void initfunc(void);
void run(void);

int main(int argc, char **argv) {
  if (argc > 2) {
    goto argc2;
  }
  printf("Invalid arguments passed\n***\tprogram terminated\n");
  exit(0);
argc2:
  initTrip(&trip);
  while (i < argc) {
    int len = strlen(*(argv + i));
    if (**(argv + i) != '-' && len > 2) {
      strcat(message, *(argv + i));
    }
    i++;
  }
  reset(1);
  while (i < argc) {
    if (**(argv + i) == '-') {
      sort(*(argv + i));
    }
    i++;
  }

  run();
  exit(0);
}

void initTrip(Trip *T_trip) {
  T_trip->yTrip = 0;
  T_trip->mTrip = 0;
  T_trip->iTrip = 0;
}
void input(char *str) { strcat(command, str); }

void reset(int num) { i = num; };
void sort(char *str2) {
  switch (*(str2 + 1)) {
  case 'm':
    trip.mTrip = 1;
    break;
  case 'y':
    trip.yTrip = 1;
    strcat(push, " && git push");
    break;
  case 'i':
    trip.iTrip = 1;
    initfunc();
    break;
  case 'n':
    trip.yTrip = 0;
    break;
  default:
    printf("-%c is an invalid command", *(str2 + 1));
    break;
  }
};

void initfunc(void) {
  strcat(temp, "cd $(pwd) && git init && git add . && git commit -m '");
}

void run(void) {
  if (trip.iTrip) {
    input(temp);
  } else {
    input("cd $(pwd) && git add . && git commit -m '");
  }

  if (trip.mTrip) {
    input(message);
    input("'");
  } else {
    printf(" no commit message made\nThis is highly discouraged\n\tinputing "
           "the default message...\n");
    input("no message'");
  }
  if (trip.yTrip) {
    input(push);
  }
  system(command);
}

// temp holds git init
// message holds the message
// push holds the git puah info
