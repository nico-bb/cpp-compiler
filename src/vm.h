#pragma once
#include "ast.h"
#include "core/list.h"
#include "core/string.h"
#include "core/table.h"
#include "value.h"

struct Vm {
  Table<String, Value> variables;
};

Vm make_vm(Allocator allocator);

void run_program(Vm &vm, List<Statement> &ast, Error &err);

Value eval_expr(Expression &expr, Error &err);
