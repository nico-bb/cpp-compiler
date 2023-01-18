#include "ast.h"
#include "core/mem.h"
#include "core/table.h"
#include "stdio.h"
#include "token.h"

int main() {
  auto heap = heap_allocator();

  auto arena_size = Megabyte * 10;
  Arena arena;
  init_arena(
      &arena,
      static_cast<char *>(heap.alloc(arena_size)),
      arena_size);

  auto allocator = arena_allocator(&arena);

  auto table = make_table<String, size_t>(allocator, 2);

  auto str = string_from_literal("hello");
  table[str] = 10;

  auto str2 = string_from_literal("world");
  table[str2] = 20;

  for (auto it = table_iter(table); it.ok(); it.next()) {
    printf("%s\n", string_cstr(it.key));
  }

  auto source = make_string(allocator, "1 + 2", 5);
  auto lexer = Lexer {
    .source = source,
    .current = 0,
  };

  while (true) {
    auto token = next_token(lexer);

    auto token_str = token_kind_str(token.kind);
    printf("%s\n", string_cstr(token_str));
    if (token.kind == Token_Eof) {
      break;
    }
  }

  auto left = allocator.new_clone(
      Literal_Expression { .value = Value { .kind = Boolean_Value_Kind, .boolean = false } });

  auto right = allocator.new_clone(
      Literal_Expression { .value = Value { .kind = Boolean_Value_Kind, .boolean = true } });

  auto b = allocator.new_clone(
      Binary_Expression { .op = Operator_And, .left = literal_expr(left), .right = literal_expr(right) });

  auto binary = binary_expr(b);

  Error err;
  auto result = eval_expr(binary, err);

  allocator.free_all();
  heap.free(arena.buf);

  return 0;
}