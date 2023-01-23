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

Statement expression_stmt(Expression_Statement *stmt) {
  auto result = Statement {
    .kind = Expression_Statement_Kind,
    .expr_stmt = stmt,
  };

  return result;
}

Statement var_decl(Var_Declaration *decl) {
  auto result = Statement {
    .kind = Var_Declaration_Kind,
    .var_decl = decl,
  };

  return result;
}