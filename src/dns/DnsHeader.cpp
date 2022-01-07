#include "DnsHeader.hpp"

#include <spdlog/fmt/fmt.h>

#include <stdexcept>
DnsHeader::DnsHeader(BytePacketBuffer& buffer) {
  id_ = buffer.read_u16();
  uint8_t flags_a = buffer.read();
  recursion_desired_ = (flags_a & (1 << 0)) > 0;
  truncated_message_ = (flags_a & (1 << 1)) > 0;
  authoritative_answer_ = (flags_a & (1 << 2)) > 0;
  opcode_ = (flags_a >> 3) & 0x0F;
  response_ = (flags_a & (1 << 7)) > 0;

  uint8_t flags_b = buffer.read();
  if (auto v = rcode_from_num(flags_b & 0x0F)) {
    rescode_ = *v;
  } else {
    throw std::runtime_error("Invalid rcode");
  }
  checking_disabled_ = (flags_b & (1 << 4)) > 0;
  authed_data_ = (flags_b & (1 << 5)) > 0;
  z_ = (flags_b & (1 << 6)) > 0;
  recursion_available_ = (flags_b & (1 << 7)) > 0;

  questions_ = buffer.read_u16();
  answers_ = buffer.read_u16();
  authoritative_entries_ = buffer.read_u16();
  resource_entries_ = buffer.read_u16();
}

void DnsHeader::write(BytePacketBuffer& buffer) const {
  buffer.write_u16(id_);
  buffer.write(static_cast<uint8_t>(recursion_desired_) |
               (static_cast<uint8_t>(truncated_message_) << 1) |
               (static_cast<uint8_t>(authoritative_answer_) << 2) |
               (static_cast<uint8_t>(opcode_) << 3) |
               (static_cast<uint8_t>(response_) << 7));
  buffer.write(static_cast<uint8_t>(rescode_) |
               (static_cast<uint8_t>(checking_disabled_) << 4) |
               (static_cast<uint8_t>(authed_data_) << 5) |
               (static_cast<uint8_t>(z_) << 6) |
               (static_cast<uint8_t>(recursion_available_) << 7));
  buffer.write_u16(questions_);
  buffer.write_u16(answers_);
  buffer.write_u16(authoritative_entries_);
  buffer.write_u16(resource_entries_);
}

std::optional<DnsHeader::ResultCode> DnsHeader::rcode_from_num(int num) {
  if (num < static_cast<int>(ResultCode::NOERROR) ||
      num > static_cast<int>(ResultCode::REFUSED)) {
    return {};
  }
  return ResultCode(num);
}
