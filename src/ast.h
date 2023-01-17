#pragma once
#include "error.h"
#include "token.h"
#include "value.h"

struct Literal_Expression;
struct Binary_Expression;

enum Expression_Kind {
  Literal_Expression_Kind,
  Binary_Expression_Kind,
};

struct Expression {
  Token token;
  Expression_Kind kind;
  union {
    Literal_Expression *literal;
    Binary_Expression *binary;
  };
};

struct Literal_Expression {
  Value value;
};

enum Binary_Operator {
  Operator_And,
  Operator_Or,
  Operator_Plus,
  Operator_Minus,
};

struct Binary_Expression {
  Binary_Operator op;
  Expression left;
  Expression right;
};

Expression literal_expr(Literal_Expression *lit);

Expression binary_expr(Binary_Expression *bin);

Value eval_expr(Expression &expr, Error &err);

/*
  Statements
*/

struct Expression_Statement;
struct Var_Declaration;
struct Assignment_Statement;

enum Statement_Kind {
  Expression_Statement_Kind,
  Var_Declaration_Kind,
  Assignment_Statement_Kind,
};

struct Statement {
  Token token;
  Statement_Kind kind;
  union {
    Expression_Statement *expr_stmt;
    Var_Declaration *var_decl;
    Assignment_Statement *assign_stmt;
  };
};

struct Expression_Statement {
  Expression expr;
};

struct Var_Declaration {
  String identifier;
  Token eq_token;
  Expression initializer;
};

struct Assignment_Statement {
  Token eq_token;
  Expression lhs;
  Expression rhs;
};