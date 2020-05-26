#include <stdio.h>
#include <string.h>
#include "codegen.h"
#include "parser.h"
#include "error.h"

void gen_lval(Node *node) {
  printf("#lval\n");
  if (node->kind != ND_LVAR)
    error("lval is not variable.");
  printf("  mov rax, rbp\n");
  printf("  sub rax, %d\n", node->offset);
  printf("  push rax\n");
}

int label_num = 0;

char *arg_reg[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};

void gen_function() {
  for (int fi = 0; g_function[fi]; fi++) {
    char name[256] = {};
    strncpy(name, g_function[fi]->name, g_function[fi]->len);
    printf("%s:\n", name);
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    // 引数
    printf("#args move reg to stack\n");
    for (size_t ai = g_function[fi]->args_size; ai > 0; ai--) {
      printf("  push %s\n", arg_reg[ai-1]);
    }
    // ローカル変数分のスタックを確保（26個分）
    printf("  sub rsp, 208\n");

    gen(g_function[fi]->block);
  }
}

void gen(Node *node) {
  if (node->kind == ND_CALL) {
    printf("#function call\n");
    // 引数をセット
    printf("#set params\n");
    Node *p = node->param;
    int pnum = 0;
    while (p) {
      gen(p);
      p = p->next;
      pnum++;
    }
    while (pnum--) {
      printf("  pop rax\n");
      printf("  mov %s, rax\n", arg_reg[pnum]);
    }
    // RSPを16の倍数に調整
    printf("#align rsp\n");
    char buff[256] = {0};
    strncpy(buff, node->ident, node->len);
    label_num++;
    printf("  mov rax, rsp\n");
    printf("  and rax, 15\n");
    printf("  jnz .Laligned%d\n", label_num);
    printf("  mov rax, 0\n");
    printf("  call %s\n", buff);
    printf("  jmp .Lend%d\n", label_num);
    printf(".Laligned%d:\n", label_num);
    printf("  sub rsp, 8\n");
    printf("  mov rax, 0\n");
    printf("  call %s\n", buff);
    printf("  add rsp, 8\n");
    printf(".Lend%d:\n", label_num);
    printf("  push rax\n");
    return;
  }

  if (node->kind == ND_BLOCK) {
    printf("#block start\n");
    Node *body = node->body;
    while (body) {
      gen(body);
      printf("  pop rax\n");
      body = body->next;
    }
    printf("#block end\n");
    return;
  }

  if (node->kind == ND_FOR) {
    printf("#for\n");
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
    printf("#while\n");
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
    printf("#if\n");
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
    printf("#return\n");
    printf("  pop rax\n");
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return;
  }

  if (node->kind == ND_ADDR) {
    printf("#address\n");
    gen_lval(node->lhs);
    return;
  }

  if (node->kind == ND_DEREF) {
    printf("#dereference\n");
    gen(node->lhs);
    printf("  pop rax\n");
    printf("  mov rax, [rax]\n");
    printf("  push rax\n");
    return;
  }

  switch (node->kind) {
  case ND_NUM:
    printf("#num\n");
    printf("  push %d\n", node->val);
    return;
  case ND_LVAR:
    gen_lval(node);
    printf("#lvar\n");
    printf("  pop rax\n");
    printf("  mov rax, [rax]\n");
    printf("  push rax\n");
    return;
  case ND_ASSIGN:
    if (node->lhs->kind == ND_DEREF)
      gen(node->lhs->lhs);
    else
      gen_lval(node->lhs);
    gen(node->rhs);
    printf("#assign\n");
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

  printf("#bi-ope\n");
  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch (node->kind) {
  case ND_ADD:
    printf("#add\n");
    if (node->lhs->kind == ND_LVAR && node->lhs->type->ty == PTR) {
      printf("  imul rdi, 8\n");
      printf("  sub rax, rdi\n");
      break;
    }
    printf("  add rax, rdi\n");
    break;
  case ND_SUB:
    printf("#sub\n");
    if (node->lhs->kind == ND_LVAR && node->lhs->type->ty == PTR) {
      printf("  imul rdi, 8\n");
      printf("  add rax, rdi\n");
      break;
    }
    printf("  sub rax, rdi\n");
    break;
  case ND_MUL:
    printf("#mul\n");
    printf("  imul rax, rdi\n");
    break;
  case ND_DIV:
    printf("#div\n");
    printf("  cqo\n");
    printf("  idiv rdi\n");
    break;
  case ND_EQUAL:
    printf("#eq\n");
    printf("  cmp rax, rdi\n");
    printf("  sete al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_NOT_EQUAL:
    printf("#ne\n");
    printf("  cmp rax, rdi\n");
    printf("  setne al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_GREAT:
    printf("#gt\n");
    printf("  cmp rdi, rax\n");
    printf("  setl al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_LESS:
    printf("#lt\n");
    printf("  cmp rax, rdi\n");
    printf("  setl al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_GREAT_EQUAL:
    printf("#ge\n");
    printf("  cmp rdi, rax\n");
    printf("  setle al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_LESS_EQUAL:
    printf("#le\n");
    printf("  cmp rax, rdi\n");
    printf("  setle al\n");
    printf("  movzb rax, al\n");
    break;
  default:
    break;
  }
  printf("  push rax\n");
}
