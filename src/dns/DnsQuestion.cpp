#include "DnsQuestion.hpp"

#include <glog/logging.h>

#include <stdexcept>

bool DnsQuestion::from_buffer(BytePacketBuffer &buffer) {
  if (auto v = buffer.read_qname()) {
    name_ = *v;
  } else {
    return false;
  }
  if (auto v = rtype_from_num(buffer.read_u16())) {
    rtype_ = *v;
  } else {
    return false;
  }
  if (rtype_ == RecordType::OPT) {
    LOG(INFO) << "Pseudo resource records not supported";
    return false;
  }
  if (buffer.read_u16() != static_cast<uint16_t>(RecordClass::IN)) {
    DLOG(INFO) << "Invalide record class";
    return false;
  }
  rclass_ = RecordClass::IN;
  return true;
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
