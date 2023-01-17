#pragma once
#include <core/mem.h>

#define DEFAULT_INIT_CAP 16
#define DEFAULT_GROWTH_FACTOR 2

template <typename T>
struct List {
  Allocator &allocator;
  T *data;
  size_t length;
  size_t cap;

  T &operator[](size_t index) {
    return data[index];
  }
};

template <typename T>
List<T> make_list(Allocator &allocator, size_t cap = DEFAULT_INIT_CAP) {
  return List {
    .allocator = allocator,
    .data = static_cast<T *> allocator.alloc(sizeof(T) * cap),
    .length = 0,
    .cap = cap,
  };
}

template <typename T>
void delete_list(List<T> &list) {
  list.allocator.free(list.data);
  list.data = nullptr;
  list.length = 0;
  list.cap = 0;
}

template <typename T>
void append_list(List<T> &list, T item) {
  if (list.length == list.cap) {
    grow_list(list);
  }

  list.data[list.length] = item;
}

template <typename T>
void clear_list(List<T> &list) {
  list.length = 0;
}

template <typename T>
void grow_list(List<T> &list) {
  auto old_memory = list.data;
  auto old_cap = list.cap;

  list.cap *= DEFAULT_GROWTH_FACTOR;
  list.data = list.allocator.resize(sizeof(T) * list.cap, old_memory, sizeof(T) * old_cap);
}