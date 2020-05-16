#include <stdio.h>

int piyo() {
  return 42;
}

int hoge(int a, int b, int c) {
  puts("call hoge SUCCESS!!");
  return a+2*b+3*c;
}

int fuge(int a, int b) {
  puts("call fuge!!!");
  return a*(b+1);
}
