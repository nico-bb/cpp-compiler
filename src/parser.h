#pragma once
#include "ast.h"
#include "core/list.h"
#include "core/string.h"
#include "token.h"

struct Parser {
  Allocator allocator;
  String source;
  List<Statement> ast;

  Lexer lexer;
  Token current;
  Token previous;
};

Token &consume_token(Parser &parser);

List<Statement> parse_source_string(Allocator &allocator, String source);

Statement parse_next_statement(Parser &parser);