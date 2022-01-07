#include "RecordClass.hpp"

#include <spdlog/spdlog.h>

std::optional<RecordClass> rclass_from_num(int num) {
  if (num < static_cast<int>(RecordClass::IN) ||
      num > static_cast<int>(RecordClass::HS)) {
    spdlog::info("Invalid value of {}", num);
    return {};
  }
  return RecordClass{num};
}