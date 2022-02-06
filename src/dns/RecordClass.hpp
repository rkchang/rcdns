#pragma once
#include <optional>

enum class RecordClass { IN = 1, CS, CH, HS };
std::optional<RecordClass> rclass_from_num(int num);
