#include "parser.h"
#include "core/slice.h"

Token &consume_token(Parser &parser) {
  parser.previous = parser.current;
  parser.current = next_token(parser.lexer);
  return parser.current;
}

List<Statement> parse_source_string(Allocator &allocator, String source, Error &err) {
  auto parser = Parser {
    .allocator = allocator,
    .source = source,
    .ast = make_list<Statement>(allocator),
    .lexer = Lexer {
        .source = source,
    },
  };

  while (true) {
    auto leading = consume_token(parser);
    if (leading.kind == Token_Newline) {
      continue;
    } else if (leading.kind == Token_Eof) {
      break;
    }

    auto stmt = parse_next_statement(parser, err);
  }

  return parser.ast;
}

Statement parse_next_statement(Parser &parser, Error &err) {
  static Token_Kind inv_tokens[] = {
    Token_Comma,
    Token_Dot,
    Token_Plus,
    Token_Minus,
    Token_Star,
    Token_Slash,
  };
  static Slice<Token_Kind> invalid_tokens = slice_from_ptr(inv_tokens, 6);

  if (slice_contains(invalid_tokens, parser.current.kind)) {
    err.kind = Error_Invalid_Token;
    err.stage = Error_Stage_Parsing;
    return {};
  }

  switch (parser.current.kind) {
  default:
    parse_expression(parser, err);
    break;
  }
}

Expression parse_expression(Parser &parser, Error &err) {
}