#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

struct Strings{
char add[100], command[1024], message[500], push[30], init[100], commit[100];
};

int i = 1;

struct Trips{
  bool iTrip, yTrip, mTrip;
};

Trips *trip = new Trips;
Strings *str= new Strings;

void initStr();
void initTrip(Trips *T_trip);
void input(char *str);
void reset(int num);
void sort(char *str2);
void run(void);

int main(int argc, char **argv) {
  if (argc > 2) {
    goto argc2;
  }
  printf("Invalid arguments passed\n***\tprogram terminated\nArguments:\n-i\t init a git repo\n-m \"message\"\t add a message\n-y\tpush to online git repo");
  exit(0);
argc2:
  initTrip(&trip);
  initStr(&str);
  while (i < argc) {
    int len = strlen(*(argv + i));
    if (**(argv + i) != '-' && len > 1) {
      strcat(str->message, *(argv + i));
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

  run(&str);
  exit(0);
}

void initStr(Strings *istr){
  strcat(istr->command,"cd $(pwd) && ");
  strcat(istr->add," git add . ");
  strcat(istr->push," git push ");
  strcat(istr->commit," git commit ");
  strcat( istr->init, " git init ");
}

void initTrip(Trips *T_trip) {
  T_trip->yTrip = 0;
  T_trip->mTrip = 0;
  T_trip->iTrip = 0;
}

void input(char *istr) { strcat(str->command, istr); }

void reset(int num) { i = num; };
void sort(char *str2) {
  switch (*(str2 + 1)) {
  case 'm':
    trip->mTrip = 1;
    break;
  case 'y':
    trip->yTrip = 1;
    break;
  case 'i':
    trip->iTrip = 1;
    break;
  case 'n':
    trip->yTrip = 0;
    break;
  default:
    printf("-%c is an invalid command", *(str2 + 1));
    break;
  }
};


void run(Strings *str){
  if( trip->iTrip ){
    input(str->init,);
  }


  system(str->command);
}
//add = git add .
//push = git push
//init = git init
//commit = git commit
//message = git(message)


