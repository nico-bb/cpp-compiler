#pragma once
#include "core/string.h"

enum Error_Kind {
  Error_None,
  Error_Invalid_Token,
  Error_Mismatched_Types,
};

enum Error_Stage {
  Error_Stage_Lexing,
  Error_Stage_Parsing,
  Error_Stage_Checking,
  Error_Stage_Runtime,
};

struct Error {
  Error_Kind kind;
  Error_Stage stage;
  String details;

  bool ok() {
    return kind == Error_None;
  }
};