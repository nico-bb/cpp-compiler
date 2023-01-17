#pragma once
#include "core/mem.h"
#include <stdarg.h>

struct String {
  Allocator allocator;
  size_t length;
  char const *data;

  const char operator[](size_t index) {
    return data[index];
  }

  bool operator==(String &other) {
    if (length != other.length) {
      return false;
    }

    for (size_t i = 0; i < length; i += 1) {
      if (data[i] != other[i]) {
        return false;
      }
    }

    return true;
  }
};

inline String make_string(Allocator &a, char const *str, size_t length) {
  auto buf = static_cast<char *>(a.alloc(length * sizeof(char)));
  for (size_t i = 0; i < length; i += 1) {
    buf[i] = str[i];
  }

  auto result = String {
    .allocator = a,
    .length = length,
    .data = buf,
  };

  return result;
}

inline String slice_string(String &from, size_t start, size_t end) {
  auto result = String {
    .allocator = from.allocator,
    .length = end - start,
    .data = from.data + start,
  };

  return result;
}

// inline String sprintf(Allocator &allocator, const char *fmt, ...) {

// }