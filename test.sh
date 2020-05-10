#!/bin/bash

assert() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  gcc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]
  then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 42 'a = 42;'
assert 43 'a = 42 + 1;'
assert 126 'a = 42 * (2 + 1);'
assert 3 'a = 2; b = 1; c = a + b;'
assert 14 'a = 2; b = 3; c = 4 * a + b * 2;'
assert 3 'c=a=b=a=a=a=b=b=c=c=1;x=a+b+c;'
assert 0 '(x-a)*(x-b)*(x-c)*(x-d)*(x-e)*(x-f)*(x-g)*(x-h)*(x-i)*(x-j)*(x-k)*(x-l)*(x-m)*(x-n)*(x-o)*(x-p)*(x-q)*(x-r)*(x-s)*(x-t)*(x-u)*(x-v)*(x-w)*(x-x)*(x-y)*(x-z);'
assert 0 '0;'
assert 42 '42;'
assert 123 '123;'
assert 41 " 12 + 34 - 5;"
assert 125 " 1+ 2  + 3 + 100-3 -2   +24 ;"
assert 95 " 1* 2  + 3 + 100/3 *2   +24 ;"
assert 47 "5+6*7;"
assert 15 "5*(9-6);"
assert 4 "(3+ 5) / 2;"
assert 4 " + (13+ - 5) / 2;"
assert 16 " 1-3 * - 5;"
assert 16 "200/6/2;"
assert 1 "1 == 1;"
assert 0 "0 == 1;"
assert 0 "1 != 1;"
assert 1 "0 != 1;"
assert 0 "1 < 1;"
assert 1 "0 < 1;"
assert 0 "1 > 1;"
assert 1 "1 > 0;"
assert 1 "1 <= 1;"
assert 1 "0 <= 1;"
assert 0 "2 <= 1;"
assert 1 "1 >= 1;"
assert 1 "2 >= 1;"
assert 0 "0 >= 1;"
assert 1 '1+-3+15/7+1!=(8/2)>=9<-1;'
assert 0 '0 <3 <3 <3 *(0==1==0)*  (8 <0> 2 <0> 8) + (((8<=-8)));'

echo "OK"
