#include "DnsQuestion.hpp"

#include <glog/logging.h>

#include <stdexcept>

DnsQuestion::DnsQuestion(BytePacketBuffer &buffer) {
  if (auto v = buffer.read_qname()) {
    name_ = *v;
  } else {
    throw std::runtime_error("Invalid qname");
  }
  if (auto v = rtype_from_num(buffer.read_u16())) {
    rtype_ = *v;
  } else {
    throw std::runtime_error("Invalid record type");
  }
  if (rtype_ == RecordType::OPT) {
    LOG(INFO) << "Pseudo resource records not supported";
    return;
  }
  if (buffer.read_u16() != static_cast<uint16_t>(RecordClass::IN)) {
    throw std::runtime_error("Invalid record class");
  }
  rclass_ = RecordClass::IN;
}

void DnsQuestion::write(BytePacketBuffer &buffer) const {
  buffer.write_qname(name_);
  buffer.write_u16(static_cast<uint16_t>(rtype_));
  buffer.write_u16(static_cast<uint16_t>(rclass_));
}
std::ostream &operator<<(std::ostream &os, const DnsQuestion &question) {
  os << "["
     << " name_: " << question.name_
     << " rtype_: " << static_cast<int>(question.rtype_)
     << " rclass_: " << static_cast<int>(question.rclass_) << "]";
  return os;
}
