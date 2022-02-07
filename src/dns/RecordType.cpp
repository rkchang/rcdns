#include "RecordType.hpp"

#include <glog/logging.h>

std::optional<RecordType> rtype_from_num(int num) {
  if (num < static_cast<int>(RecordType::UNKNOWN) ||
      num > static_cast<int>(RecordType::OPT)) {
    LOG(INFO) << "Invalid record type " << num;
    return {};
  }
  return RecordType{num};
}