#include "RecordType.hpp"

#include <spdlog/spdlog.h>

std::optional<RecordType> rtype_from_num(int num) {
  if (num < static_cast<int>(RecordType::UNKNOWN) ||
      num > static_cast<int>(RecordType::OPT)) {
    spdlog::info("Invalid record type {}", num);
    return {};
  }
  return RecordType{num};
}