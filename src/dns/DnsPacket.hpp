#pragma once

#include <ostream>
#include <string>
#include <vector>

#include "BytePacketBuffer.hpp"
#include "DnsHeader.hpp"
#include "DnsQuestion.hpp"
#include "DnsRecord.hpp"

class DnsPacket {
 private:
  std::optional<DnsRecord> get_unresolved_nameserver(std::string &qname);

 public:
  DnsPacket() = default;
  bool from_buffer(BytePacketBuffer &buffer);
  void write(BytePacketBuffer &buffer);
  friend std::ostream &operator<<(std::ostream &os, const DnsPacket &packet);

  DnsHeader header_;
  std::vector<DnsQuestion> questions_;
  std::vector<DnsRecord> answers_;
  std::vector<DnsRecord> authorities_;
  std::vector<DnsRecord> resources_;
};