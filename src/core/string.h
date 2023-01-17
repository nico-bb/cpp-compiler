#include <core/mem.h>

struct String {
  Allocator &allocator;
  size_t length;
  size_t capacity;
  char const *str;
};

String make_string(Allocator &a, char const *str) {
}