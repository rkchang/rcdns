#pragma once

#include <cstdint>
#include <ostream>
#include <string>

#include "BytePacketBuffer.hpp"
#include "RecordClass.hpp"
#include "RecordType.hpp"

class DnsQuestion {
public:
  DnsQuestion() = default;
  DnsQuestion(BytePacketBuffer &buffer);
  void write(BytePacketBuffer &buffer) const;
  friend std::ostream &operator<<(std::ostream &os,
                                  const DnsQuestion &question);

  std::string name_;
  RecordType rtype_;
  RecordClass rclass_;
};