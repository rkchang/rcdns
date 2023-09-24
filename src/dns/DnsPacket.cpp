#include "DnsPacket.hpp"
#include <glog/logging.h>

#include <ostream>

#include "BytePacketBuffer.hpp"
#include "DnsHeader.hpp"
#include "DnsRecord.hpp"

DnsPacket::DnsPacket(BytePacketBuffer &buffer) : header_(buffer) {
  for (auto i = 0; i < header_.questions_; i++) {
    DnsQuestion qtion{};
    if (!qtion.from_buffer(buffer)) {
      LOG(WARNING) << "Failed to parse DnsRecord, DROPPING";
    }
    questions_.push_back(qtion);
  }
  for (auto i = 0; i < header_.answers_; i++) {
    DnsRecord rec{};
    if (!rec.from_buffer(buffer)) {
      LOG(WARNING) << "Failed to parse DnsRecord, DROPPING";
    }
    answers_.push_back(rec);
  }
  for (auto i = 0; i < header_.authoritative_entries_; i++) {
    DnsRecord rec{};
    if (!rec.from_buffer(buffer)) {
      LOG(WARNING) << "Failed to parse DnsRecord, DROPPING";
    }
    authorities_.push_back(rec);
  }
  for (auto i = 0; i < header_.resource_entries_; i++) {
    DnsRecord rec{};
    if (!rec.from_buffer(buffer)) {
      LOG(WARNING) << "Failed to parse DnsRecord, DROPPING";
    }
    resources_.push_back(rec);
  }
}

void DnsPacket::write(BytePacketBuffer &buffer) {
  header_.questions_ = questions_.size();
  header_.answers_ = answers_.size();
  header_.authoritative_entries_ = authorities_.size();
  header_.resource_entries_ = resources_.size();

  header_.write(buffer);
  for (auto &question : questions_) {
    question.write(buffer);
  }
  for (auto &record : answers_) {
    record.write(buffer);
  }
  for (auto &record : authorities_) {
    record.write(buffer);
  }
  for (auto &record : resources_) {
    record.write(buffer);
  }
}

std::ostream &operator<<(std::ostream &os, const DnsPacket &packet) {
  os << "[\n"
     << " header_: " << packet.header_ << "\n"
     << " questions_: [";
  for (const auto &elem : packet.questions_) {
    os << elem;
  }
  os << "]\n";

  os << " answers_: [";
  for (const auto &elem : packet.answers_) {
    os << elem;
  }
  os << "]\n";

  os << " authorities_: [";
  for (const auto &elem : packet.authorities_) {
    os << elem;
  }
  os << "]\n";

  os << " resources_: [";
  for (const auto &elem : packet.resources_) {
    os << elem;
  }
  os << "]\n";

  os << "]\n";
  return os;
}
