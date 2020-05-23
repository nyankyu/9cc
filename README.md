# 9cc
ruiさんの[低レイヤを知りたい人のためのCコンパイラ作成入門](https://www.sigbus.info/compilerbook)にそってcコンパイラ(9cc)を実装する。9ccはCのソースコードからアセンブラコードを作成する。

## 用語
- 構文解析(syntactic analysis/parse) 文章を形態素に分け、それらの言語上の関係を図式化などによって明確化する手続き。ここでは、ソースコードを字句に分割し、構文木を作成する操作。構文解析を行う機械やプログラムを構文解析器(parser)という。構文解析器を生成するツールとして[yacc](https://ja.wikipedia.org/wiki/Yacc)が有名。
- 字句解析(Lexical Analysis) 構文解析の前半の処理。文章を字句(token)に分割する処理。ここでは、ソースコードをキーワードやリテラルなどに分割してリスト化する処理。
- AST(Abstract Syntax Tree) 抽象構文木。構文木から言語の意味に関係のない、カッコなどの要素を取り除いた物。
- BNF/EBNF(Backus-Naur Form/Extended Backus-Naur Form)文脈自由文法を表現するメタ文法記法。ここでは、C言語の文法を表現するのに使用する。
- LL(1)パーサ。トークンを1つだけ先読みする再帰下降パーサ。LL(1)パーサがパースできる文法をLL(1)文法という。
- スタックマシン スタックをデータ保持領域として持っているコンピュータ。スタックマシンにおける2項演算は、スタックから2つの要素をポップして、それらの演算結果をプッシュする。カッコや演算子に優先順位がある式の計算が簡単にできる。

## EBNF
```ebnf
program    = function*
function   = "int" ident "(" argment? ")" "{" stmt* "}"
stmt       = expr ";"
             | "{" stmt* "}"
             | if "(" expr ")" stmt ("else" stmt)?
             | for "(" expr ";" expr ";" expr ")" stmt
             | while "(" expr ")" stmt
             | "return" expr ";"
             | "int" ident ";"
expr       = assign
assign     = equality ("=" assign)?
equality   = relational ("==" relational | "!=" relational)*
relational = add ("<" add | ">" add | "<=" add | ">=" add)*
add        = mul ("+" mul | "-" mul)*
mul        = unary ("*" unary | "/" unary)*
unary      = ("+" | "-")? primary
             | "*" unary
             | "&" unary
primary    = num
             | ident
             | ident ("(" param? ")")
             | "(" expr ")"
param      = expr ("," expr)*
argment    = "int" ident ("," "int" ident)*
```

## 未実装
- 型（扱え値は符号無し整数のみ）
  - ポインタ
  - 構造体
  - 共用体
  - 文字列
  - 配列
- プリプロセッサ
- switch文
- break
- continue
- ラベル
- グローバル変数
- コメント
- スコープ
- enum
- typedef
- goto
- 浮動小数

## 制限
- 引数:6個
- 変数:26個
- 関数:256個
- 識別子:256文字

## 演算子
演算子と優先順位
|演算子|結合性|
|---|---|
|+（単項）, -（単項）|右|
|*, /|左|
|+, -|左|
|<, <=, >, >=|左|
|==, !=|左|
|=|右|


## 構成
### tokenizer
Cのソースコードを字句解析して、tokenのリストを作成する。
```c
typedef enum {
  TK_RESERVED,
  TK_IDENT,
  TK_NUM,
  TK_EOF,
} TokenKind;

struct Token {
  TokenKind kind;
  Token *next;
  int val;
  char *str;
  int len;
};
```

### parser
tokenizerが作成したtokenリストを、EBNFにそって解析してASTを作成する。
```c
typedef enum {
  ND_ADD,         // +
  ND_SUB,         // -
  ND_MUL,         // *
  ND_DIV,         // /
  ND_NUM,         // integer
  ND_EQUAL,       // ==
  ND_NOT_EQUAL,   // !=
  ND_GREAT,       // >
  ND_LESS,        // <
  ND_GREAT_EQUAL, // >=
  ND_LESS_EQUAL,  // <=
  ND_ASSIGN,      // =
  ND_LVAR,        // local variable
  ND_RETURN,      // return
  ND_IF,          // if
  ND_ELSE,        // else
} NodeKind;

struct Node {
  NodeKind kind;
  Node *lhs;
  Node *rhs;
  int val;
  int offset;     // offset from RBP
};
```

### code generator
parserが作成したASTをrootノードから読んで、assemblyコードを出力する。

## アセンブリコード
### main関数のみからなるプログラム。そのmain関数は整数「42」を返すだけ。
```assembly
.intel_syntax noprefix
.global main
main:
  mov rax, 42
  ret
```

### 整数の四則演算
```assembly
; raxに4+61の結果が格納される
  mov rax, 4
  mov rdi, 61
  add rax, rdi
; raxに6-2の結果が格納される
  mov rax, 6
  mov rdi, 2
  sub rax, rdi
; raxに10*5の結果が格納される
  mov rax, 10
  mov rdi, 5
  imul rax, rdi
; raxに70/3の結果が格納される
  mov rax, 70
  mov rdi, 3
  cqo
  idiv rdi
```
