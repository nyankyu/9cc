CFLAGS=-std=c11 -g -static

9cc: 9cc.c \
  token.c \
  error.c \
  ast.c \
  parser.c \
  generator.c

test: 9cc
	./test.sh

clean:
	rm -f 9cc *.o *~ tmp*

.PHONY: test clean
