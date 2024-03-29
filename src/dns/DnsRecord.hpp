#pragma once
#include <asio.hpp>
#include <cstdint>
#include <ostream>
#include <string>
#include <variant>

#include "BytePacketBuffer.hpp"
#include "RecordClass.hpp"
#include "RecordType.hpp"

class DnsRecord {
private:
  void write_ip(uint16_t size);

public:
  struct AData {
    asio::ip::address_v4 ip;
    friend std::ostream &operator<<(std::ostream &os, const AData &payload);
  };
  struct NSData {
    std::string host;
    friend std::ostream &operator<<(std::ostream &os, const NSData &payload);
  };
  struct CNAMEData {
    std::string host;
    friend std::ostream &operator<<(std::ostream &os, const CNAMEData &payload);
  };
  struct MXData {
    uint16_t priority;
    std::string host;
    friend std::ostream &operator<<(std::ostream &os, const MXData &payload);
  };
  struct AAAAData {
    asio::ip::address_v4 ip;
    friend std::ostream &operator<<(std::ostream &os, const AAAAData &payload);
  };

  DnsRecord() = default;
  bool from_buffer(BytePacketBuffer &buffer);
  bool write(BytePacketBuffer &buffer) const;
  friend std::ostream &operator<<(std::ostream &os, const DnsRecord &record);

  std::string domain_;
  RecordType rtype_;
  RecordClass rclass_;
  uint32_t ttl_;
  uint16_t data_len_;
  std::variant<std::monostate, AData, NSData, CNAMEData, MXData, AAAAData>
      data_;
};