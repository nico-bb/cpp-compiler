#pragma once
#include "core/string.h"

enum Token_Kind {
  Token_Eof,
  Token_Newline,

  Token_Number,

  Token_Comma,
  Token_Dot,
  Token_Plus,
  Token_Minus,
  Token_Star,
  Token_Slash,
};

struct Token {
  Token_Kind kind;
  size_t line;
  size_t column;
  size_t start;
  size_t end;
  String text;
};

struct Lexer {
  String source;
  size_t current;
  size_t line;
  size_t column;
};

String token_kind_str(Token_Kind kind);

Token next_token(Lexer &lexer);

const char advance(Lexer &lexer);

const char peek(Lexer &lexer);

bool is_eof(Lexer &lexer);

bool is_letter(const char c);

bool is_number(const char c);

void skip_whitespaces(Lexer &lexer);