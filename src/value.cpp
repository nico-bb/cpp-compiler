#include "value.h"

bool check_value_types(Value_Kind expected, Value &v1, Value &v2) {
  return (v1.kind == expected && v2.kind == expected);
}