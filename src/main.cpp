#include "parser.h"
#include "stdio.h"
#include "vm.h"

void debug_tokens(String &source);

int main() {
  auto heap = heap_allocator();

  auto arena_size = Megabyte * 10;
  Arena arena;
  init_arena(
      &arena,
      static_cast<char *>(heap.alloc(arena_size)),
      arena_size);

  auto allocator = arena_allocator(&arena);

  auto source = make_string(allocator, "var i = 0", 9);

  debug_tokens(source);

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

void debug_tokens(String &source) {
  auto lexer = Lexer {
    .source = source,
  };

  while (true) {
    auto token = next_token(lexer);

    if (token.kind == Token_Eof) {
      break;
    }

    auto token_str = token_kind_str(token.kind);
    printf("%s\n", string_cstr(token_str));
  }
}
