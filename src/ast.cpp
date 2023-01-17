#include "ast.h"

Expression literal_expr(Literal_Expression *lit) {
  return Expression {
    .kind = Literal_Expression_Kind,
    .literal = lit,
  };
}

Expression binary_expr(Binary_Expression *bin) {
  return Expression {
    .kind = Binary_Expression_Kind,
    .binary = bin,
  };
}

Value eval_expr(Expression &expr, Error &err) {
  Value result;

  switch (expr.kind) {
  case Literal_Expression_Kind:
    result = expr.literal->value;
    break;
  case Binary_Expression_Kind:
    auto left = eval_expr(expr.binary->left, err);
    if (!err.ok()) {
      return {};
    }

    auto right = eval_expr(expr.binary->right, err);
    if (!err.ok()) {
      return {};
    }

    Value_Kind expected;
    switch (expr.binary->op) {
    case Operator_And:
    case Operator_Or:
      expected = Boolean_Value_Kind;
      break;
    case Operator_Plus:
    case Operator_Minus:
      expected = Number_Value_Kind;
      break;
    }
    if (!check_value_types(expected, left, right)) {
      err.kind = Error_Mismatched_Types;
      return {};
    }

    switch (expr.binary->op) {
    case Operator_And:
      result = Value {
        .kind = Boolean_Value_Kind,
        .boolean = left.boolean && right.boolean,
      };
      break;
    case Operator_Or:
      result = Value {
        .kind = Boolean_Value_Kind,
        .boolean = left.boolean || right.boolean,
      };
      break;
    case Operator_Plus:
      result = Value {
        .kind = Number_Value_Kind,
        .number = left.number + right.number,
      };
      break;
    case Operator_Minus:
      result = Value {
        .kind = Number_Value_Kind,
        .number = left.number - right.number,
      };
      break;
    }
    break;
  }

  return result;
}
