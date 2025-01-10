#pragma once

#include <ostream>
#include <vector>

#include "BytePacketBuffer.hpp"
#include "DnsHeader.hpp"
#include "DnsQuestion.hpp"
#include "DnsRecord.hpp"
#include "Constants.hpp"

class DnsPacket {
public:
  DnsPacket() = default;
  void from_buffer(BytePacketBuffer &buffer);
  void from_buffer(std::array<uint8_t, constants::DNS_PACKET_SIZE> &buffer);
  void write(BytePacketBuffer &buffer);
  friend std::ostream &operator<<(std::ostream &os, const DnsPacket &packet);

  DnsHeader header_;
  std::vector<DnsQuestion> questions_;
  std::vector<DnsRecord> answers_;
  std::vector<DnsRecord> authorities_;
  std::vector<DnsRecord> resources_;
};