#include <stdlib.h>

size_t align_forward(size_t offset, size_t align) {
  const auto padding = offset % align;
  if (padding > 0) {
    return offset + (align - padding);
  }
  return offset;
}

struct Allocator {
  void *data;
  void *(*alloc)(Allocator &a, size_t size);
  void *(*resize)(Allocator &a, size_t size, void *old_memory, size_t old_size);
  void (*free)(Allocator &a, void *memory);
  void (*free_all)(Allocator &a);
};

Allocator heap_allocator() {
  auto alloc_proc = [](Allocator &a, size_t size) -> void * {
    return malloc(size);
  };
  auto resize_proc = [](Allocator &a, size_t size, void *old_memory, size_t old_size) -> void * {
    a.free(a, old_memory);
    a.alloc(a, size);
  };
  auto free_proc = [](Allocator &a, void *memory) { 
    free(memory);
  };

  auto heap_allocator = Allocator{
    .data = nullptr,
    .alloc = alloc_proc,
    .resize = resize_proc,
    .free = free_proc,
    .free_all = [](Allocator &a) {},
  };
  return heap_allocator;
}

struct Arena {
  char *buf;
  size_t alignment;
  size_t offset;
  size_t used;
};

Allocator arena_allocator(Arena *arena) {
  auto alloc_proc = [](Allocator &a, size_t size) -> void * {
    auto arena = static_cast<Arena *>(a.data);
    arena->offset = align_forward(arena->offset, arena->alignment);
    auto mem = arena->buf + arena->offset;
    arena->offset += size;

    return static_cast<void *>(mem);
  };
  auto resize_proc = [](Allocator &a, size_t size, void *old_memory, size_t old_size) -> void * {
    return a.alloc(a, size);
  };
  auto free_proc = [](Allocator &a, void *memory) {};
  auto free_all_proc = [](Allocator &a) {
    auto arena = static_cast<Arena *>(a.data);
    arena->offset = 0;
  };


  auto arena_allocator = Allocator {
    .data = arena,
    .alloc = alloc_proc,
    .resize = resize_proc,
    .free = free_proc,
    .free_all = free_all_proc,
  };

  return arena_allocator;
}

