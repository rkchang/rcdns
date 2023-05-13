#pragma once
#include <optional>
#include <string>

enum class RecordType {
  UNKNOWN,
  A = 1,
  NS = 2,
  CNAME = 5,
  MX = 15,
  AAAA = 28,
  OPT = 41
};

std::optional<RecordType> rtype_from_num(int num);
std::string name_from_rtype(RecordType &rt);
