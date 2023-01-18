#pragma once

template <typename T>
struct Maybe {
  union {
    char dummy;
    T value;
  };
  bool has_value;

  Maybe()
      : dummy(0)
      , has_value(false) { }
};
