#include <ast.h>
#include <core/string.h>

int main() {
  auto heap = heap_allocator();

  auto arena_size = Megabyte * 10;
  Arena arena;
  init_arena(
      &arena,
      static_cast<char *>(heap.alloc(arena_size)),
      arena_size);

  auto allocator = arena_allocator(&arena);

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