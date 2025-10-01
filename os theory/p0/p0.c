/*
Zachary Carter
CS-3060-X02 Fall 2025
Program 0
*/

#include <stdio.h>

int main(int argc, char *argv[]) {
 int i;

 printf("Zachary Carter - Program 0\n");

 for (i = 0; i < argc; i++) {
  printf("Argument #%d: ", i);
  printf("%s\n", argv[i]);
 }
 printf("Number of arguments printed: %d\n", argc);

 return 0;
}
