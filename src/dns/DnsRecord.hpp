#pragma once
#include <asio.hpp>
#include <cstdint>
#include <string>
#include <variant>
#include <ostream>

#include "BytePacketBuffer.hpp"
#include "RecordClass.hpp"
#include "RecordType.hpp"

class DnsRecord {
 private:
  void write_ip(uint16_t size);

 public:
  struct AData {
    asio::ip::address_v4 ip;
  };
  struct NSData {
    std::string host;
  };
  struct CNAMEData {
    std::string host;
  };
  struct MXData {
    uint16_t priority;
    std::string host;
  };
  struct AAAAData {
    asio::ip::address_v4 ip;
  };

  DnsRecord(BytePacketBuffer& buffer);
  bool write(BytePacketBuffer& buffer) const;
  friend std::ostream &operator<<(std::ostream &os, const DnsRecord &record);

  std::string domain_;
  RecordType rtype_;
  RecordClass rclass_;
  uint32_t ttl_;
  uint16_t data_len_;
  std::optional<std::variant<AData, NSData, CNAMEData, MXData, AAAAData>> data_;
};