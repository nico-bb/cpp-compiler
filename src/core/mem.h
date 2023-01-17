#pragma once
#include <stdlib.h>
#include <string.h>

#define DEFAULT_ALIGN 4

const size_t Kilobyte = 1024;
const size_t Megabyte = 1024 * Kilobyte;
const size_t Gigabyte = 1024 * Megabyte;

inline size_t align_forward(size_t offset, size_t align) {
  const auto padding = offset % align;
  if (padding > 0) {
    return offset + (align - padding);
  }
  return offset;
}

struct Allocator {
  void *data;
  void *(*alloc_proc)(Allocator &a, size_t size);
  void *(*resize_proc)(Allocator &a, size_t size, void *old_memory, size_t old_size);
  void (*free_proc)(Allocator &a, void *memory);
  void (*free_all_proc)(Allocator &a);

  void *alloc(size_t size) {
    return alloc_proc(*this, size);
  }

  void *resize(size_t size, void *old_memory, size_t old_size) {
    return resize_proc(*this, size, old_memory, old_size);
  }

  void free(void *memory) {
    free_proc(*this, memory);
  }

  void free_all() {
    free_all_proc(*this);
  }

  template <typename T>
  T *new_raw() {
    auto memory = alloc(sizeof(T));
    return static_cast<T *>(memory);
  }

  template <typename T>
  T *new_clone(T &&from) {
    auto t = static_cast<T *>(alloc(sizeof(T)));
    *t = from;
    return t;
  }
};

inline Allocator heap_allocator() {
  auto alloc_proc = [](Allocator &a, size_t size) -> void * {
    return malloc(size);
  };
  auto resize_proc = [](Allocator &a, size_t size, void *old_memory, size_t old_size) -> void * {
    auto new_memory = a.alloc(size);
    memcpy(new_memory, old_memory, old_size);
    a.free(old_memory);

    return new_memory;
  };
  auto free_proc = [](Allocator &a, void *memory) {
    free(memory);
  };

  auto heap_allocator = Allocator {
    .data = nullptr,
    .alloc_proc = alloc_proc,
    .resize_proc = resize_proc,
    .free_proc = free_proc,
    .free_all_proc = [](Allocator &a) {},
  };
  return heap_allocator;
}

struct Arena {
  char *buf;
  size_t total_size;
  size_t alignment;
  size_t offset;
};

inline void init_arena(Arena *arena, char *buf, size_t size) {
  arena->buf = buf;
  arena->total_size = size;
  arena->alignment = DEFAULT_ALIGN;
  arena->offset = 0;
}

inline Allocator arena_allocator(Arena *arena) {
  auto alloc_proc = [](Allocator &a, size_t size) -> void * {
    auto _arena = static_cast<Arena *>(a.data);
    _arena->offset = align_forward(_arena->offset, _arena->alignment);
    auto mem = _arena->buf + _arena->offset;
    _arena->offset += size;

    return static_cast<void *>(mem);
  };
  auto resize_proc = [](Allocator &a, size_t size, void *old_memory, size_t old_size) -> void * {
    auto new_memory = a.alloc(size);
    memcpy(new_memory, old_memory, old_size);
    return new_memory;
  };
  auto free_proc = [](Allocator &a, void *memory) {};
  auto free_all_proc = [](Allocator &a) {
    auto _arena = static_cast<Arena *>(a.data);
    _arena->offset = 0;
  };

  auto arena_allocator = Allocator {
    .data = arena,
    .alloc_proc = alloc_proc,
    .resize_proc = resize_proc,
    .free_proc = free_proc,
    .free_all_proc = free_all_proc,
  };

  return arena_allocator;
}
