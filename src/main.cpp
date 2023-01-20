#include "core/mem.h"
#include "core/table.h"
#include "parser.h"
#include "stdio.h"
#include "vm.h"

int main() {
  auto heap = heap_allocator();

  auto arena_size = Megabyte * 10;
  Arena arena;
  init_arena(
      &arena,
      static_cast<char *>(heap.alloc(arena_size)),
      arena_size);

  auto allocator = arena_allocator(&arena);

  auto source = make_string(allocator, "1 + 2", 5);

  auto err = Error {
    .kind = Error_None,
  };
  auto ast = parse_source_string(allocator, source, err);
  if (!err.ok()) {
    printf("Error: %s\n", err.kind);
  }

  auto vm = make_vm(allocator);
  run_program(vm, ast, err);

  allocator.free_all();
  heap.free(arena.buf);

  return 0;
}