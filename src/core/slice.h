#pragma once
#include "core/mem.h"

template <typename T>
struct Slice {
  Allocator allocator;
  T *data;
  size_t length;

  T &operator[](size_t index) {
    return data[index];
  }
};

template <typename T>
inline Slice<T> slice_from_ptr(T *data, size_t length) {
  return Slice {
    .data = data,
    .length = length,
  };
}

template <typename T>
inline bool slice_contains(Slice<T> &slice, T key) {
  for (size_t i = 0; i < slice.length; i += 1) {
    if (key == slice.data[i]) {
      return true;
    }
  }

  return false;
}