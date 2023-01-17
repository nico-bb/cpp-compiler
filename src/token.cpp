#include "token.h"

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
  default: {
    if (is_number(c)) {
      token.kind = Token_Number;

      while (!is_eof(lexer)) {
        auto next = peek(lexer);
        if (!is_number(next)) {
          break;
        }

        advance(lexer);
      }
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