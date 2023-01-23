#include "token.h"
#include "core/pair.h"
#include "core/slice.h"

const char advance(Lexer &lexer);

const char peek(Lexer &lexer);

bool is_eof(Lexer &lexer);

bool is_letter(const char c);

bool is_number(const char c);

void lex_number(Lexer &lexer);

void lex_identifier(Lexer &lexer);

void skip_whitespaces(Lexer &lexer);

Token_Kind keyword_token(String &string);

/*
  Procedures Implementation
*/

String token_kind_str(Token_Kind kind) {
  String result;

  switch (kind) {
  case Token_Eof:
    result = string_from_literal("Token_Eof");
    break;
  case Token_Newline:
    result = string_from_literal("Token_Newline");
    break;
  case Token_Var:
    result = string_from_literal("Token_Var");
    break;
  case Token_Identifier:
    result = string_from_literal("Token_Identifier");
    break;
  case Token_Number:
    result = string_from_literal("Token_Number");
    break;
  case Token_Comma:
    result = string_from_literal("Token_Comma");
    break;
  case Token_Dot:
    result = string_from_literal("Token_Dot");
    break;
  case Token_Plus:
    result = string_from_literal("Token_Plus");
    break;
  case Token_Minus:
    result = string_from_literal("Token_Minus");
    break;
  case Token_Star:
    result = string_from_literal("Token_Star");
    break;
  case Token_Slash:
    result = string_from_literal("Token_Slash");
    break;
  case Token_Eq:
    result = string_from_literal("Token_Eq");
    break;
  }

  return result;
}

Token next_token(Lexer &lexer) {
  Token token;

  skip_whitespaces(lexer);

  if (is_eof(lexer)) {
    token.kind = Token_Eof;
    return token;
  }

  token.start = lexer.current;
  token.line = lexer.line;
  token.column = lexer.column;

  auto c = advance(lexer);

  switch (c) {
  case '\n':
    token.kind = Token_Newline;
    lexer.line += 1;
    lexer.column = 0;
    break;
  case ',':
    token.kind = Token_Comma;
    break;
  case '.':
    token.kind = Token_Dot;
    break;
  case '+':
    token.kind = Token_Plus;
    break;
  case '-':
    token.kind = Token_Minus;
    break;
  case '*':
    token.kind = Token_Star;
    break;
  case '/':
    token.kind = Token_Slash;
    break;
  case '=':
    token.kind = Token_Eq;
    break;
  default: {
    if (is_number(c)) {
      token.kind = Token_Number;
      lex_number(lexer);
    } else if (is_letter(c)) {
      lex_identifier(lexer);
      auto identifier = slice_string(lexer.source, token.start, lexer.current);
      token.kind = keyword_token(identifier);
    }
    break;
  }
  }

  token.end = lexer.current;
  token.text = slice_string(lexer.source, token.start, token.end);
  return token;
}

const char advance(Lexer &lexer) {
  lexer.current += 1;
  lexer.column += 1;
  return lexer.source[lexer.current - 1];
}

const char peek(Lexer &lexer) {
  return lexer.source[lexer.current];
}

bool is_eof(Lexer &lexer) {
  return lexer.current >= lexer.source.length;
}

bool is_letter(const char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool is_number(const char c) {
  return c >= '0' && c <= '9';
}

void lex_number(Lexer &lexer) {
  while (!is_eof(lexer)) {
    auto next = peek(lexer);
    if (!is_number(next)) {
      break;
    }

    advance(lexer);
  }
}

void lex_identifier(Lexer &lexer) {
  while (!is_eof(lexer)) {
    auto next = peek(lexer);

    // FIXME: Disallow leading and trailing underscores
    if (!is_letter(next) && next != '_') {
      break;
    }
    advance(lexer);
  }
}

void skip_whitespaces(Lexer &lexer) {
  while (!is_eof(lexer)) {
    auto c = peek(lexer);

    if (c == ' ' || c == '\t' || c == '\r') {
      advance(lexer);
    } else {
      break;
    }
  }
}

Token_Kind keyword_token(String &keyword) {
  static const size_t pair_count = 1;
  static Pair<String, Token_Kind> p[pair_count] = {
    { .first = string_from_literal("var"), .second = Token_Var },
  };
  static Slice<Pair<String, Token_Kind>> pairs = slice_from_ptr(p, pair_count);

  for (size_t i = 0; i < pairs.length; i += 1) {
    auto pair = pairs[i];
    if (pair.first == keyword) {
      return pair.second;
    }
  }

  return Token_Identifier;
}