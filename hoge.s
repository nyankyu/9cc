#====== tokenize =======
# kind:0, str:int, len:3
# kind:1, str:main, len:4
# kind:0, str:(, len:1
# kind:0, str:), len:1
# kind:0, str:{, len:1
# kind:0, str:int, len:3
# kind:0, str:*, len:1
# kind:1, str:p, len:1
# kind:0, str:;, len:1
# kind:0, str:int, len:3
# kind:1, str:p1, len:2
# kind:0, str:;, len:1
# kind:0, str:int, len:3
# kind:1, str:p2, len:2
# kind:0, str:;, len:1
# kind:0, str:int, len:3
# kind:0, str:*, len:1
# kind:1, str:a, len:1
# kind:0, str:;, len:1
# kind:1, str:p, len:1
# kind:0, str:=, len:1
# kind:0, str:&, len:1
# kind:1, str:p1, len:2
# kind:0, str:;, len:1
# kind:1, str:p1, len:2
# kind:0, str:=, len:1
# kind:2, val:1
# kind:0, str:;, len:1
# kind:1, str:p2, len:2
# kind:0, str:=, len:1
# kind:2, val:2
# kind:0, str:;, len:1
# kind:1, str:a, len:1
# kind:0, str:=, len:1
# kind:1, str:p, len:1
# kind:0, str:+, len:1
# kind:2, val:1
# kind:0, str:;, len:1
# kind:0, str:return, len:6
# kind:0, str:*, len:1
# kind:1, str:a, len:1
# kind:0, str:;, len:1
# kind:0, str:}, len:1
# kind:3, str:, len:1
#====== made AST =======
.intel_syntax noprefix
.global main
main:
  push rbp
  mov rbp, rsp
#args move reg to stack
  sub rsp, 208
#block start
#lval
  mov rax, rbp
  sub rax, 8
  push rax
#address
#lval
  mov rax, rbp
  sub rax, 16
  push rax
#assign
  pop rdi
  pop rax
  mov [rax], rdi
  push rdi
  pop rax
#lval
  mov rax, rbp
  sub rax, 16
  push rax
#num
  push 1
#assign
  pop rdi
  pop rax
  mov [rax], rdi
  push rdi
  pop rax
#lval
  mov rax, rbp
  sub rax, 24
  push rax
#num
  push 2
#assign
  pop rdi
  pop rax
  mov [rax], rdi
  push rdi
  pop rax
#lval
  mov rax, rbp
  sub rax, 32
  push rax
#lval
  mov rax, rbp
  sub rax, 8
  push rax
#lvar
  pop rax
  mov rax, [rax]
  push rax
#num
  push 1
#bi-ope
  pop rdi
  pop rax
#imul rdi, 8
#add
  add rax, rdi
  push rax
#assign
  pop rdi
  pop rax
  mov [rax], rdi
  push rdi
  pop rax
#dereference
#lval
  mov rax, rbp
  sub rax, 32
  push rax
#lvar
  pop rax
  mov rax, [rax]
  push rax
  pop rax
  mov rax, [rax]
  push rax
#return
  pop rax
  mov rsp, rbp
  pop rbp
  ret
  pop rax
#block end
