#pragma once
#include "core/list.h"
#include "core/mem.h"
#include "core/types.h"
#include <math.h>
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

  size_t operator%(size_t mod) {
    auto sum = 0;
    for (size_t i = 0; i < length; i += 1) {
      sum += data[i];
    }
    return sum % mod;
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

inline String string_from_literal(const char *literal) {
  auto result = String {
    .length = strlen(literal),
    .data = literal,
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

inline const char *string_cstr(String &string) {
  return string.data;
}

inline f64 string_parse_f64(String &string) {
  f64 result = 0;
  f64 places = 0;
  bool in_decimal = false;

  for (size_t i = 0; i < string.length; i += 1) {
    const auto c = string[i];

    if (c == '.') {
      if (in_decimal) {
        return 0;
      }

      in_decimal = true;
      continue;
    }

    if (c >= '0' && c <= '9') {
      result = (result * 10) + static_cast<f64>(c - '0');

      if (in_decimal) {
        places -= 1;
      }
      continue;
    }

    return 0;
  }

  result *= pow(10, places);
  return result;
}

inline String int_to_string(Allocator &allocator, int n) {
  size_t length = 0;

  for (size_t rem = n; rem > 0; rem /= 10) {
    length += 1;
  }

  // Alloc buffer
  char *buf = static_cast<char *>(allocator.alloc(length * sizeof(char)));

  size_t i = 0;
  for (size_t rem = n; rem > 0; rem /= 10) {
    auto digit = rem % 10;
    rem /= 10;

    buf[length - i - 1] = char(digit) + 48;
    i += 1;
  }

  auto result = String {
    .allocator = allocator,
    .length = length,
    .data = buf,
  };

  return result;
}

struct String_Builder {
  List<char> buf;
};

inline String_Builder make_string_builder(Allocator &allocator, size_t cap) {
  auto builder = String_Builder {
    .buf = make_list<char>(allocator),
  };
  return builder;
}

inline void destroy_string_builder(String_Builder &builder) {
  delete_list(builder.buf);
}

inline void reset_string_builder(String_Builder &builder) {
  clear_list(builder.buf);
}

inline String builder_to_string(String_Builder &builder) {
  auto result = String {
    .allocator = builder.buf.allocator,
    .length = builder.buf.length,
    .data = builder.buf.data,
  };

  return result;
}

// FIXME: Implement a stack allocator

inline void builder_write_string(String_Builder &builder, String &string) {
  for (size_t i = 0; i < string.length; i += 1) {
    auto c = string[i];
    append_list(builder.buf, c);
  }
}

inline void builder_null_terminate(String_Builder &builder) {
  append_list(builder.buf, '\0');
}

inline void builder_write_byte(String_Builder &builder, char byte) {
  append_list(builder.buf, byte);
}

inline void builder_write_int(String_Builder &builder, int n) {
  size_t start = builder.buf.length;
  size_t length = 0;

  for (size_t rem = n; rem > 0; rem /= 10) {
    append_list(builder.buf, ' ');
    length += 1;
  }

  // Alloc buffer
  size_t i = 0;
  for (size_t rem = n; rem > 0; rem /= 10) {
    auto digit = rem % 10;
    rem /= 10;

    auto index = (start + length) - (i + 1);
    builder.buf[index] = char(digit) + 48;
    i += 1;
  }
}

// FIXME: request some sort of writer interface instead of directly
// printing to stdout
inline void print_string(String &string) {
}

// inline String sprintf(Allocator &allocator, const char *fmt, ...) {

// }