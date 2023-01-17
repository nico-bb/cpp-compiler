#pragma once

enum Error_Kind {
  Error_None,
  Error_Mismatched_Types,
};

struct Error {
  Error_Kind kind;

  bool ok() {
    return kind == Error_None;
  }
};