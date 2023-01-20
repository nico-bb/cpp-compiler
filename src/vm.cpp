#include "vm.h"

Vm make_vm(Allocator allocator) {
  auto vm = Vm {
    .variables = make_table<String, Value>(allocator, 24),
  };

  return vm;
}

void run_stmt(Vm &vm, Statement stmt, Error &err);

void run_program(Vm &vm, List<Statement> &ast, Error &err) {
  for (size_t i = 0; i < ast.length; i += 1) {
    auto stmt = ast[i];
    run_stmt(vm, stmt, err);
  }
}

void run_stmt(Vm &vm, Statement stmt, Error &err) {
  switch (stmt.kind) {
  case Expression_Statement_Kind:
    eval_expr(stmt.expr_stmt->expr, err);
  default:
    break;
  }
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