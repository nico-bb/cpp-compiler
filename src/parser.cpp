#include "parser.h"
#include "core/slice.h"

Expression parse_expression(Parser &parser, Precedence prec, Error &err);
Expression parse_number_literal(Parser &parser, Error &err);
Expression parse_binary_expression(Parser &parser, Expression left, Error &err);

void init_parser_rules(Parser &parser) {
  parser.rules[Token_Number] = Parser_Rule {
    .prec = Precedence_Lowest,
    .prefix_proc = parse_number_literal,
    .infix_proc = nullptr,
  };
  parser.rules[Token_Plus] = Parser_Rule {
    .prec = Precedence_Term,
    .prefix_proc = nullptr,
    .infix_proc = parse_binary_expression,
  };
}

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
    .rules = make_table<Token_Kind, Parser_Rule>(allocator, 10),
    .lexer = Lexer {
        .source = source,
    },
  };
  init_parser_rules(parser);

  while (true) {
    auto leading = consume_token(parser);
    if (leading.kind == Token_Newline) {
      continue;
    } else if (leading.kind == Token_Eof) {
      break;
    }

    auto stmt = parse_next_statement(parser, err);
    if (!err.ok()) {
      return parser.ast;
    }

    append_list(parser.ast, stmt);
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

  auto result = Statement {
    .token = parser.current,
  };
  auto token = parser.current;
  switch (token.kind) {
  default: {
    auto expr = parse_expression(parser, Precedence_Lowest, err);

    if (!err.ok()) {
      return {};
    }
    auto expr_stmt = parser.allocator.new_raw<Expression_Statement>();
    expr_stmt->expr = expr;

    result = expression_stmt(expr_stmt);
    result.token = token;
    break;
  }
  }

  return result;
}

Expression parse_expression(Parser &p, Precedence prec, Error &err) {
  Expression result;

  // Parse the first expression
  consume_token(p);
  if (table_has_key(p.rules, p.previous.kind)) {
    auto rule = p.rules[p.previous.kind];
    if (rule.prefix_proc != nullptr) {
      result = rule.prefix_proc(p, err);

      if (!err.ok()) {
        return {};
      }
    }
  }

  // FIXME: might cause bugs with the current table implementation
  while (p.current.kind != Token_Eof && prec < p.rules[p.current.kind].prec) {
    consume_token(p);
    if (table_has_key(p.rules, p.previous.kind)) {
      auto rule = p.rules[p.previous.kind];
      if (rule.infix_proc != nullptr) {
        result = rule.infix_proc(p, result, err);

        if (!err.ok()) {
          return {};
        }
      }
    }
  }

  return result;
}

Expression parse_number_literal(Parser &p, Error &err) {
  auto literal = p.allocator.new_raw<Literal_Expression>();
  literal->value = Value {
    .kind = Number_Value_Kind,
    .number = string_parse_f64(p.previous.text),
  };
  auto result = literal_expr(literal);
  result.token = p.previous;

  return result;
}

Expression parse_binary_expression(Parser &p, Expression left, Error &err) {
  auto binary = p.allocator.new_raw<Binary_Expression>();

  auto op_token = p.previous;
  switch (op_token.kind) {
  case Token_Plus:
    binary->op = Operator_Plus;
    break;
  case Token_Minus:
    binary->op = Operator_Minus;
    break;
  default:
    break;
  }

  binary->left = left;
  binary->right = parse_expression(
      p,
      p.rules[p.previous.kind].prec,
      err);

  auto result = binary_expr(binary);
  result.token = op_token;

  return result;
}
