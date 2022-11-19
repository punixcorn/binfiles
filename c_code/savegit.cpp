#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

struct Strings{
char add[100], command[1024], message[500], push[30], init[100], commit[100];
};

int i = 1;

struct Trips{
  bool iTrip, yTrip, mTrip,cTrip,fTrip;
};

Trips *trip = new Trips;
Strings *str= new Strings;

void initStr(Strings* istr);
void initTrip(Trips *T_trip);
void input(char *istr);
void reset(int num);
void sort(char *str2);
void run(Strings *wstr);

int main(int argc, char **argv) {
  if ( argc == 2 ){
    if( strcmp("-i",*argv+1) ){
        goto argc2;
    }else{
      ;
    }
  }
  if (argc > 2) {
    goto argc2;
  }
  printf("Invalid arguments passed\n***\tprogram terminated\nArguments:\n-i\t\tiinit a git repo\n-c\tcommit\n-m \"message\"\tadd a message\n-y\t\tpush to online git repo\n-f\tsee the command runned");
  exit(0);
argc2:
  initTrip(trip);
  initStr(str);
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

  run(str);
  exit(0);
}

void initStr(Strings *istr){
  strcat(istr->command,"cd $(pwd) && ");
  strcat(istr->add," git add . && ");
  strcat(istr->push," && git push ");
  strcat(istr->commit," git commit -m ");
  strcat( istr->init, " git init ");
}

void initTrip(Trips *T_trip) {
  T_trip->yTrip = 0;
  T_trip->mTrip = 0;
  T_trip->iTrip = 0;
  T_trip->cTrip = 0;
  T_trip->fTrip = 0;
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
  case 'c':
    trip->cTrip = 1;
    break;
  case 'f':
    trip->fTrip = 1;
    break;
  default:
    printf("-%c is an invalid command", *(str2 + 1));
    break;
  }
};

/*
   if -i is 1, then it should be first hence "git init"
   if -m is there then we add git add . && git commit -m "message"
   if -y is there then we add git push
*/

void run(Strings *wstr){
  //init trip
  if( trip->iTrip ){
    input(wstr->init);
    if( trip->cTrip ){
      input(" && ");
    }
  }
  //commit trip
  if ( trip->cTrip ){
    input(wstr->add);
    input(wstr->commit);
  }
    
  //message trip = 1
  if (trip->mTrip ){
    input("'");
    input(wstr->message);
    input("'");
  }
  
  //commit trip =1 , message trip = 0 , init trip = 1
  if( (trip->cTrip) && !(trip->mTrip) && (trip->iTrip) ){
    input("'init commit'");
  }

  //message trip = 0 and init trip = 0 
  if ( (trip->cTrip) && !(trip->mTrip) && !(trip->iTrip)){ 
  input("'commit made'");
  }

  //save trip
  if( trip->yTrip){
  input(wstr->push);
  }
  if( trip->fTrip ){
    printf("\ncommand: \e[31m%s\n\n\e[0m",str->command);
  }
  system(str->command);
}
//add = git add .
//push = git push
//init = git init
//commit = git commit
//message = git(message)


