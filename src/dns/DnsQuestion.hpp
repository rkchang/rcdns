#pragma once

#include <cstdint>
#include <string>

#include "BytePacketBuffer.hpp"
#include "RecordClass.hpp"
#include "RecordType.hpp"

class DnsQuestion {
 public:
  DnsQuestion() = default;
  DnsQuestion(BytePacketBuffer& buffer);
  void write(BytePacketBuffer& buffer) const;
  std::string name_;
  RecordType rtype_;
  RecordClass rclass_;
};