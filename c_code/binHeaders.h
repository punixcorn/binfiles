#ifndef h_binHeaders_h
#define h_binHeaders_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFERSIZE 100
#define BUFFERSIZEX 500
extern char command[BUFFERSIZE];
extern char error[BUFFERSIZEX];
void input(char *arg);
void stderror(char *arg);
#endif
