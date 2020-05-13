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
assert 1 'if (1) return 1; return 42;'
assert 42 'if (0) return 1; return 42;'
assert 1 'a = 42; if (1) a = 1; if (1) return a; return a+10;'
assert 42 'a = 42; if (0) a = 1; if (1) return a; return a+10;'
assert 11 'a = 42; if (1) a = 1; if (0) return a; return a+10;'
assert 52 'a = 42; if (0) a = 1; if (0) return a; return a+10;'
assert 1 'if (1) a=1; else a=2; return a;'
assert 2 'if (0) a=1; else a=2; return a;'
assert 42 'iff = (42); return iff;'
assert 42 'return 42;'
assert 0 'return 0; return 42;'
assert 3 'returnn = 3; return returnn;'
assert 3 'return 1+2;'
assert 42 'return aaaa = 42;'
assert 43 'return bbb = 42 + 1;'
assert 126 'return ccc = 42 * (2 + 1);'
assert 3 'aaa = 2; bbb = 1; return ccc = aaa + bbb;'
assert 14 'aaa = 2; bbb = 3; return ccc = 4 * aaa + bbb * 2;'
assert 6 'hoge = 1; hogehoge = 2; h = 3; return hoge+hogehoge+h;'
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
