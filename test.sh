#!/bin/bash

assert() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  gcc -o tmp tmp.s func.o
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

assert 12 'int main() {a = 12; b = &a; return *b;}'
assert 42 'int aaa() {a = 21; return a;} int main() {a = 2; return aaa() * a;}'
assert 42 'int aaa() {a = 21; return a;} int bbb() {return 2;} int main() {return aaa() * bbb();}'
assert 8 'int aaa(int x, int y) {a = x + y; return a;} int main() {return aaa(3, 5);}'
assert 0 'int aaa(int x) {if (x == 0) {return 0;} return aaa(x-1);} int main() {return aaa(10);}'
assert 89 'int aaa(int x) {if (x <= 1) {return 1;} return aaa(x-1) + aaa(x-2);} int main() {return aaa(10);}'
assert 42 'int main() {return piyo();}'
assert 36 'int main() {a = 2; return hoge(a, a+3, a*4);}'
assert 141  'int main() {return hoge(fuge(2, 3), hoge(1, 2, 3), fuge(5, 6));}'
assert 14 'int main() {{a = 1; b = 2; c = 3;} {a = a+b+c; b = b+c;} return a+b+c;}'
assert 6 'int main() {{a = 1; b = 2; c = 3; a = b * c;} return a;}'
assert 101 'int main() {b=0; c=0; for (a = 1; a < 100; a = a+1) {b=a+1; c=b+1; d=c*b;} return c;}'
assert 100 'int main() {a=1; while (a < 100) a = a+1; return a;}'
assert 1 'int main() {if (1) return 1; return 42;}'
assert 42 'int main() {if (0) return 1; return 42;}'
assert 1 'int main() {a = 42; if (1) a = 1; if (1) return a; return a+10;}'
assert 42 'int main() {a = 42; if (0) a = 1; if (1) return a; return a+10;}'
assert 11 'int main() {a = 42; if (1) a = 1; if (0) return a; return a+10;}'
assert 52 'int main() {a = 42; if (0) a = 1; if (0) return a; return a+10;}'
assert 1 'int main() {if (1) a=1; else a=2; return a;}'
assert 2 'int main() {if (0) a=1; else a=2; return a;}'
assert 42 'int main() {iff = (42); return iff;}'
assert 42 'int main() {return 42;}'
assert 0 'int main() {return 0; return 42;}'
assert 3 'int main() {returnn = 3; return returnn;}'
assert 3 'int main() {return 1+2;}'
assert 42 'int main() {return aaaa = 42;}'
assert 43 'int main() {return bbb = 42 + 1;}'
assert 126 'int main() {return ccc = 42 * (2 + 1);}'
assert 3 'int main() {aaa = 2; bbb = 1; return ccc = aaa + bbb;}'
assert 14 'int main() {aaa = 2; bbb = 3; return ccc = 4 * aaa + bbb * 2;}'
assert 6 'int main() {hoge = 1; hogehoge = 2; h = 3; return hoge+hogehoge+h;}'
assert 3 'int main() {c=a=b=a=a=a=b=b=c=c=1; return x=a+b+c;}'
assert 0 'int main() {return (x-a)*(x-b)*(x-c)*(x-d)*(x-e)*(x-f)*(x-g)*(x-h)*(x-i)*(x-j)*(x-k)*(x-l)*(x-m)*(x-n)*(x-o)*(x-p)*(x-q)*(x-r)*(x-s)*(x-t)*(x-u)*(x-v)*(x-w)*(x-x)*(x-y)*(x-z);}'
assert 41 'int main() { return 12 + 34 - 5;}'
assert 125 'int main() { return 1+ 2  + 3 + 100-3 -2   +24 ;}'
assert 95 'int main() { return 1* 2  + 3 + 100/3 *2   +24 ;}'
assert 47 'int main() {return 5+6*7;}'
assert 15 'int main() {return 5*(9-6);}'
assert 4 'int main() {return (3+ 5) / 2;}'
assert 4 'int main() {return  + (13+ - 5) / 2;}'
assert 16 'int main() {return  1-3 * - 5;}'
assert 16 'int main() {return 200/6/2;}'
assert 1 'int main() {return 1 == 1;}'
assert 0 'int main() {return 0 == 1;}'
assert 0 'int main() {return 1 != 1;}'
assert 1 'int main() {return 0 != 1;}'
assert 0 'int main() {return 1 < 1;}'
assert 1 'int main() {return 0 < 1;}'
assert 0 'int main() {return 1 > 1;}'
assert 1 'int main() {return 1 > 0;}'
assert 1 'int main() {return 1 <= 1;}'
assert 1 'int main() {return 0 <= 1;}'
assert 0 'int main() {return 2 <= 1;}'
assert 1 'int main() {return 1 >= 1;}'
assert 1 'int main() {return 2 >= 1;}'
assert 0 'int main() {return 0 >= 1;}'
assert 1 'int main() {return 1+-3+15/7+1!=(8/2)>=9<-1;}'
assert 0 'int main() {return 0 <3 <3 <3 *(0==1==0)*  (8 <0> 2 <0> 8) + (((8<=-8)));}'

echo "OK"
