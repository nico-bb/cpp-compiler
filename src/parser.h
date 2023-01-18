#pragma once
#include "ast.h"
#include "core/list.h"
#include "core/string.h"
#include "token.h"

struct Parser;

enum Precedence {
  Precedence_Lowest,
  Precedence_Assignment,
  Precedence_Term,
  Precedence_Factor,
  Precedence_Highest,
};

struct Parser_Rule {
  Precedence prec;
  Expression (*prefix_proc)(Parser &parser, Error &err);
  Expression (*infix_proc)(Parser &parser, Expression left, Error &err);
};

struct Parser {
  Allocator allocator;
  String source;
  List<Statement> ast;

  Lexer lexer;
  Token current;
  Token previous;
};

Token &consume_token(Parser &parser);

List<Statement> parse_source_string(Allocator &allocator, String source, Error &err);

Statement parse_next_statement(Parser &parser, Error &err);

Expression parse_expression(Parser &parser, Error &err);