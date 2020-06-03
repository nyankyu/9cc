#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "token.h"
#include "ast.h"
#include "util.h"
#include "error.h"

Var *g_locals = NULL;

void add_lvar(Type *type, Token *token) {
  for (Var *var = g_locals; var; var = var->next)
    if (var->len == token->len && memcmp(token->str, var->name, var->len) == 0)
      return;

  Var *lvar = calloc(1, sizeof(Var));
  lvar->next = g_locals;
  lvar->name = token->str;
  lvar->len = token->len;
  int last_offset = g_locals ? g_locals->offset : 0;
  if (type->ty == ARRAY)
    lvar->offset = last_offset + 8 * type->len;
  else
    lvar->offset = last_offset + 8;
  lvar->type = type;
  g_locals = lvar;
}

Var *find_lvar(Token *token) {
  for (Var *var = g_locals; var; var = var->next)
    if (var->len == token->len && memcmp(token->str, var->name, var->len) == 0)
      return var;
  return NULL;
}

Type *type() {
  if (!consume("int")) {
    return NULL;
  }
  Type *ty = calloc(1, sizeof(Type));
  ty->ty = INT;

  while (consume("*")) {
    Type *ptr = calloc(1, sizeof(Type));
    ptr->ty = PTR;
    ptr->ptr_to = ty;
    ty = ptr;
  }

  return ty;
}

Node *param() {
  Node head = {};
  Node *cur = &head;
  cur = cur->next = expr();
  while (!consume(")")) {
    expect(",");
    cur = cur->next = expr();
  }
  return head.next;
}

Node *primary() {
  if (consume("(")) {
      Node *node = expr();
      expect(")");
      return (node);
  }

  Token *token = consume_ident();
  if (!token) {
    return new_node_num(expect_number());
  }

  if (consume("(")) {
    Node *param_list = NULL;
    if (!consume(")")) {
      param_list = param();
    }
    return new_call(token, param_list);
  }

  Var *lvar = find_lvar(token);
  if (lvar == NULL) {
    error("宣言させていない変数です。");
  }

  if (lvar->type->ty != ARRAY) {
    return new_node_ident(lvar);
  }

  Var *array_ptr = copy_lvar(lvar);
  array_ptr->type->ty = PTR;

  if (!consume("[")) {
    return new_node_ident(array_ptr);
  }

  // a[10] => *(a+10)
  Node *node;
  Node *array = new_node_ident(array_ptr);
  Node *index = expr();
  Node *add_node = new_node(ND_ADD, array, index);
  node = new_node(ND_DEREF, add_node, NULL);
  expect("]");

  return node;
}

Node *unary() {
  if (consume("+"))
    return primary();
  if (consume("-"))
    return new_node(ND_SUB, new_node_num(0), primary());
  if (consume("&"))
    return new_node(ND_ADDR, unary(), NULL);
  if (consume("*"))
    return new_node(ND_DEREF, unary(), NULL);
  return primary();
}

Node *mul() {
  Node *node = unary();

  for (;;) {
    if (consume("*"))
      node = new_node(ND_MUL, node, unary());
    else if (consume("/"))
      node = new_node(ND_DIV, node, unary());
    else
      return (node);
  }
}

Node *add() {
  Node *node = mul();

  for (;;) {
    if (consume("+"))
      node = new_node(ND_ADD, node, mul());
    else if (consume("-"))
      node = new_node(ND_SUB, node, mul());
    else
      return (node);
  }
}

Node *relational() {
  Node *node = add();

  for (;;) {
    if (consume("<"))
      node = new_node(ND_LESS, node, add());
    else if (consume(">"))
      node = new_node(ND_GREAT, node, add());
    else if (consume("<="))
      node = new_node(ND_LESS_EQUAL, node, add());
    else if (consume(">="))
      node = new_node(ND_GREAT_EQUAL, node, add());
    else
      return (node);
  }
}

Node *equality() {
  Node *node = relational();

  for (;;) {
    if (consume("=="))
      node = new_node(ND_EQUAL, node, relational());
    else if (consume("!="))
      node = new_node(ND_NOT_EQUAL, node, relational());
    else
      return (node);
  }
}

Node *assign() {
  Node *node = equality();
  if (consume("="))
    node = new_node(ND_ASSIGN, node, assign());
  return (node);
}

Node *expr() {
  return (assign());
}

Node *stmt() {
  Node *node;

  if (consume("return")) {
    node = new_node(ND_RETURN, expr(), NULL);
    expect(";");
    return node;
  } else if (consume("if")) {
    consume("(");
    Node *if_expr = expr();
    expect(")");
    node = new_if(if_expr, stmt());

    if (consume("else")) {
      node = add_else(node, stmt());
    }
    return node;
  } else if (consume("while")) {
    consume("(");
    Node *cnd = expr();
    expect(")");
    node = new_while(cnd, stmt());
    return node;
  } else if (consume("for")) {
    consume("(");
    Node *init = expr();
    expect(";");
    Node *cnd = expr();
    expect(";");
    Node *step = expr();
    expect(")");
    node = new_for(init, cnd, step, stmt());
    return node;
  } else if (consume("{")) {
    node = new_node(ND_BLOCK, NULL, NULL);
    Node head = {};
    Node *cur = &head;
    while (!consume("}")) {
      cur = cur->next = stmt();
    }
    node->body = head.next;
    return node;
  }

  Type *ty = type();
  if (ty) {
    Token *token = consume_ident();
    if (!token)
      error("変数宣言エラー:変数名がありません。");

    if (consume("[")) {
      ty->ty = ARRAY;
      ty->ptr_to = ty;
      ty->len = expect_number();
      expect("]");
    }

    add_lvar(ty, token);
    expect(";");
    return NULL;
  }

  node = expr();
  expect(";");
  return node;
}

size_t set_args() {
  size_t args_size = 0;
  expect("(");
  while (!consume(")")) {
    Type *ty = type();
    Token *token = consume_ident();
    Var *lvar = calloc(1, sizeof(Var));
    lvar->next = g_locals;
    lvar->name = token->str;
    lvar->len = token->len;
    lvar->offset = g_locals ? g_locals->offset + 8 : 8;
    lvar->type = ty;
    g_locals = lvar;
    args_size++;
    consume(",");
  }
  return args_size;
}

Function *function() {
  Function *func = calloc(1, sizeof(Function));

  type();
  //Type *ty = type();
  Token *token = consume_ident();
  func->name = token->str;
  func->len = token->len;

  g_locals = NULL;
  func->args_size = set_args();

  expect("{");
  Node *node = new_node(ND_BLOCK, NULL, NULL);
  Node head = {};
  Node *cur = &head;
  while (!consume("}")) {
    Node *stmt_node = stmt();
    if (stmt_node == NULL)
      continue;
    cur = cur->next = stmt_node;
  }
  node->body = head.next;
  func->block = node;

  return func;
}


void program() {
  int i = 0;
  while (!at_eof()) {
    g_function[i++] = function();
  }
  g_function[i] = NULL;
}

