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
std::string name_from_rtype(RecordType &rt) {
  std::string name{};
  switch (rt) {
  case RecordType::UNKNOWN:
    name = "UNKNOWN";
    break;
  case RecordType::A:
    name = "A";
    break;
  case RecordType::NS:
    name = "NS";
    break;
  case RecordType::CNAME:
    name = "CNAME";
    break;
  case RecordType::MX:
    name = "MX";
    break;
  case RecordType::AAAA:
    name = "AAAA";
    break;
  case RecordType::OPT:
    name = "OPT";
    break;
  default:
    LOG(FATAL) << "No ostream conversion for RecordType: "
               << static_cast<int>(rt);
    break;
  }
  return name;
}
