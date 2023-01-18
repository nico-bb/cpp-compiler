#pragma once
#include "core/mem.h"
#include "core/slice.h"

template <typename K, typename V>
struct Table_Node {
  size_t index;
  bool open;
  K key;
  V value;
  Table_Node<K, V> *next;
};

template <typename K, typename V>
struct Table {
  Allocator allocator;
  Slice<Table_Node<K, V>> entries;
  size_t length;
  size_t cap;

  V &operator[](K key) {
    return index_table(*this, key);
  }
};

template <typename K, typename V>
inline Table<K, V> make_table(Allocator allocator, size_t init_cap) {
  auto table = Table {
    .allocator = allocator,
    .entries = make_slice<Table_Node<K, V>>(allocator, init_cap),
    .length = 0,
    .cap = init_cap,
  };

  for (size_t i = 0; i < init_cap; i += 1) {
    table.entries[i] = Table_Node<K, V> {
      .index = i,
      .open = true,
    };
  }
  return table;
}

template <typename K, typename V>
inline V &index_table(Table<K, V> &table, K &key) {
  if (table.length >= table.cap) {
    grow_table(table);
  }

  auto h = hash_key(key, table.cap);
  auto entry = &table.entries[h];

  if (entry->open) {
    entry->open = false;
    entry->key = key;
    entry->value = V {};
    entry->next = nullptr;
    table.length += 1;
    return entry->value;
  }

  auto found = false;
  while (!found) {
    if (entry->key == key) {
      found = true;
      break;
    } else if (entry->next == nullptr) {
      found = false;
      break;
    }

    entry = entry->next;
  }

  if (found) {
    return entry->value;
  }

  auto next = find_open_table_slot(table, entry->index);

  if (next != nullptr) {
    entry->next = next;
    next->open = false;
    next->key = key;
    next->value = V {};
    next->next = nullptr;
    table.length += 1;
    return next->value;
  }
}

template <typename K, typename V>
inline Table_Node<K, V> *find_open_table_slot(Table<K, V> &table, size_t from) {
  for (size_t i = from + 1; i != from; i = (i + 1) % table.cap) {
    if (table.entries[i].open) {
      return &table.entries[i];
    }
  }

  return nullptr;
}

template <typename K>
inline size_t hash_key(K &key, size_t cap) {
  return key % cap;
}

template <typename K, typename V>
inline void grow_table(Table<K, V> &table) {
  auto old_memory = table.entries;
  auto old_cap = table.cap;

  table.cap *= 2;
  table.entries = make_slice<Table_Node<K, V>>(table.allocator, table.cap);

  for (size_t i = 0; i < table.cap; i += 1) {
    table.entries[i] = Table_Node<K, V> {
      .index = i,
      .open = true,
    };
  }

  for (size_t i = 0; i < old_cap; i += 1) {
    const auto old_entry = &old_memory[i];
    table[old_entry->key] = old_entry->value;
  }

  delete_slice(old_memory);
}

template <typename K, typename V>
struct Table_Iterator {
  Table<K, V> &table;
  size_t iter;
  size_t index;
  K key;
  V value;

  bool ok() {
    return iter < table.length && index <= table.cap;
  }

  void next() {
    for (size_t i = index; i < table.cap; i += 1) {
      auto &entry = table.entries[i];
      if (!entry.open) {
        key = entry.key;
        value = entry.value;
        index = i + 1;
        iter += 1;
        break;
      }
    }
  }
};

template <typename K, typename V>
Table_Iterator<K, V> table_iter(Table<K, V> &table) {
  Table_Iterator<K, V> it = {
    .table = table,
    .iter = 0,
    .index = 0,
  };

  for (size_t i = 0; i < table.cap; i += 1) {
    auto &entry = table.entries[i];
    if (!entry.open) {
      it.key = entry.key;
      it.value = entry.value;
      it.index = i + 1;
      break;
    }
  }
  return it;
}