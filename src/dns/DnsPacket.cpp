#include "DnsPacket.hpp"

#include <spdlog/fmt/fmt.h>

#include <ostream>

#include "BytePacketBuffer.hpp"
#include "DnsHeader.hpp"
#include "DnsRecord.hpp"

DnsPacket::DnsPacket(BytePacketBuffer& buffer) : header_(buffer) {
  for (auto i = 0; i < header_.questions_; i++) {
    questions_.emplace_back(DnsQuestion(buffer));
  }
  for (auto i = 0; i < header_.answers_; i++) {
    answers_.emplace_back(DnsRecord(buffer));
  }
  for (auto i = 0; i < header_.authoritative_entries_; i++) {
    authorities_.emplace_back(DnsRecord(buffer));
  }
  for (auto i = 0; i < header_.resource_entries_; i++) {
    resources_.emplace_back(DnsRecord(buffer));
  }
}

void DnsPacket::write(BytePacketBuffer& buffer) {
  header_.questions_ = questions_.size();
  header_.answers_ = answers_.size();
  header_.authoritative_entries_ = authorities_.size();
  header_.resource_entries_ = resources_.size();

  header_.write(buffer);
  for (auto& question : questions_) {
    question.write(buffer);
  }
  for (auto& record : answers_) {
    record.write(buffer);
  }
  for (auto& record : authorities_) {
    record.write(buffer);
  }
  for (auto& record : resources_) {
    record.write(buffer);
  }
}