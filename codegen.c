#include <stdio.h>
#include "codegen.h"
#include "error.h"

void gen_lval(Node *node) {
  if (node->kind != ND_LVAR)
    error("lval is not variable.");
  printf("  mov rax, rbp\n");
  printf("  sub rax, %d\n", node->offset);
  printf("  push rax\n");
}

int label_num = 0;

void gen(Node *node) {
  if (node->kind == ND_FOR) {
    gen(node->init);
    printf(".Lbegin%d:\n", label_num);
    gen(node->cnd);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je  .Lend%d\n", label_num);
    gen(node->then);
    gen(node->step);
    printf("  jmp .Lbegin%d\n", label_num);
    printf(".Lend%d:\n", label_num);
    return;
  }

  if (node->kind == ND_WHILE) {
    printf(".Lbegin%d:\n", label_num);
    gen(node->cnd);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je  .Lend%d\n", label_num);
    gen(node->then);
    printf("  jmp .Lbegin%d\n", label_num);
    printf(".Lend%d:\n", label_num);
    return;
  }

  if (node->kind == ND_IF) {
    gen(node->cnd);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je  .Lelse%d\n", label_num);
    gen(node->then);
    printf("  jmp .Lend%d\n", label_num);
    printf(".Lelse%d:\n", label_num);
    if (node->els)
      gen(node->els);
    printf(".Lend%d:\n", label_num);
    label_num++;
    return;
  }

  if (node->kind == ND_RETURN) {
    gen(node->lhs);
    printf("  pop rax\n");
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return;
  }

  switch (node->kind) {
  case ND_NUM:
    printf("  push %d\n", node->val);
    return;
  case ND_LVAR:
    gen_lval(node);
    printf("  pop rax\n");
    printf("  mov rax, [rax]\n");
    printf("  push rax\n");
    return;
  case ND_ASSIGN:
    gen_lval(node->lhs);
    gen(node->rhs);
    printf("  pop rdi\n");
    printf("  pop rax\n");
    printf("  mov [rax], rdi\n");
    printf("  push rdi\n");
    return;
  default:
    break;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch (node->kind) {
  case ND_ADD:
    printf("  add rax, rdi\n");
    break;
  case ND_SUB:
    printf("  sub rax, rdi\n");
    break;
  case ND_MUL:
    printf("  imul rax, rdi\n");
    break;
  case ND_DIV:
    printf("  cqo\n");
    printf("  idiv rdi\n");
    break;
  case ND_EQUAL:
    printf("  cmp rax, rdi\n");
    printf("  sete al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_NOT_EQUAL:
    printf("  cmp rax, rdi\n");
    printf("  setne al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_GREAT:
    printf("  cmp rdi, rax\n");
    printf("  setl al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_LESS:
    printf("  cmp rax, rdi\n");
    printf("  setl al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_GREAT_EQUAL:
    printf("  cmp rdi, rax\n");
    printf("  setle al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_LESS_EQUAL:
    printf("  cmp rax, rdi\n");
    printf("  setle al\n");
    printf("  movzb rax, al\n");
    break;
  default:
    break;
  }
  printf("  push rax\n");
}
