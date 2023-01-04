#pragma once

#include <cstdint>
#include <ostream>

#include "BytePacketBuffer.hpp"

class DnsHeader {
public:
  enum class ResultCode {
    NOERROR = 0,
    FORMERR,
    SERVFAIL,
    NXDOMAIN,
    NOTIMP,
    REFUSED
  };

  DnsHeader() = default;
  explicit DnsHeader(BytePacketBuffer &buffer);
  void write(BytePacketBuffer &buffer) const;
  static std::optional<ResultCode> rcode_from_num(int num);
  friend std::ostream &operator<<(std::ostream &os, const DnsHeader &header);

  uint16_t id_;
  bool recursion_desired_;
  bool truncated_message_;
  bool authoritative_answer_;
  uint8_t opcode_;
  bool response_;

  ResultCode rescode_;
  bool checking_disabled_;
  bool authed_data_;
  bool z_;
  bool recursion_available_;
  uint16_t questions_;
  uint16_t answers_;
  uint16_t authoritative_entries_;
  uint16_t resource_entries_;
};