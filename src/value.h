#pragma once
#include "core/types.h"

enum Value_Kind {
  None_Value_Kind,
  Boolean_Value_Kind,
  Number_Value_Kind,
};

struct Value {
  Value_Kind kind;
  union {
    bool boolean;
    f64 number;
  };

  bool operator==(Value &other) {
    if (kind != other.kind) {
      return false;
    }

    auto result = false;

    switch (kind) {
    case None_Value_Kind:
      result = true;
      break;
    case Boolean_Value_Kind:
      result = boolean && other.boolean;
      break;
    case Number_Value_Kind:
      result = number == other.number;
      break;
    }

    return result;
  }
};

bool check_value_types(Value_Kind expected, Value &v1, Value &v2);