#include "RecordClass.hpp"

#include <glog/logging.h>

std::optional<RecordClass> rclass_from_num(int num) {
  if (num < static_cast<int>(RecordClass::IN) ||
      num > static_cast<int>(RecordClass::HS)) {
    LOG(INFO) << "Invalid value of " << num;
    return {};
  }
  return RecordClass{num};
}