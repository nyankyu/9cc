# 9cc
https://www.sigbus.info/compilerbook にそってcコンパイラを実装する。9ccはCのソースコードからアセンブラコードを作成する。

## 用語
- 構文解析(syntactic analysis/parse) 文章を形態素に分け、それらの言語上の関係を図式化などによって明確化する手続き。ここでは、ソースコードを字句に分割し、構文木を作成する操作。構文解析を行う機械やプログラムを構文解析器(parser)という。構文解析器を生成するツールとして[yacc](https://ja.wikipedia.org/wiki/Yacc)が有名。
- 字句解析(Lexical Analysis) 構文解析の前半の処理。文章を字句(token)に分割する処理。ここでは、ソースコードをキーワードやリテラルなどに分割してリスト化する処理。
- AST(Abstract Syntax Tree) 抽象構文木。構文木から言語の意味に関係のない、カッコなどの要素を取り除いた物。
- BNF/EBNF(Backus-Naur Form/Extended Backus-Naur Form)文脈自由文法を表現するメタ文法記法。ここでは、C言語の文法を表現するのに使用する。

- LL(1)パーサ。トークンを1つだけ先読みする再帰下降パーサ。LL(1)パーサがパースできる文法をLL(1)文法という。
- スタックマシン スタックをデータ保持領域として持っているコンピュータ。スタックマシンにおける2項演算は、スタックから2つの要素をポップして、それらの演算結果をプッシュする。カッコや演算子に優先順位がある式の計算が簡単にできる。
## BNL
```ebnf
expr    = mul ("+" mul | "-" mul)*
mul     = primary ("*" primary | "/" primary)*
primary = num | "(" expr ")"
```
