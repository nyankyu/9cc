#include <stdio.h>

int hoge(int a, int b, int c) {
  puts("call hoge SUCCESS!!");
  return a+b+c;
}

int fuge(int a, int b) {
  puts("call fuge!!!");
  return a*b;
}
