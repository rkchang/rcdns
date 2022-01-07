#pragma once

#include <string>
#include <vector>

#include "BytePacketBuffer.hpp"
#include "DnsHeader.hpp"
#include "DnsQuestion.hpp"
#include "DnsRecord.hpp"

class DnsPacket {
 public:
  DnsPacket() = default;
  DnsPacket(BytePacketBuffer& buffer);
  void write(BytePacketBuffer& buffer);

  DnsHeader header_;
  std::vector<DnsQuestion> questions_;
  std::vector<DnsRecord> answers_;
  std::vector<DnsRecord> authorities_;
  std::vector<DnsRecord> resources_;
};