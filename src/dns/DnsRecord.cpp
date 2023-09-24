#include "DnsRecord.hpp"
#include "RecordType.hpp"

#include <glog/logging.h>

#include <stdexcept>
#include <string>

bool DnsRecord::from_buffer(BytePacketBuffer &buffer) {
  if (auto v = buffer.read_qname()) {
    domain_ = *v;
  } else {
    LOG(WARNING) << "Invalid name";
    return false;
  }
  if (auto v = rtype_from_num(buffer.read_u16())) {
    rtype_ = *v;
  } else {
    rtype_ = RecordType::UNKNOWN;
  }
  switch (rtype_) {
  case RecordType::A: {
    if (buffer.read_u16() != 1) {
      LOG(WARNING) << "Invalid record class";
      return false;
    }
    rclass_ = RecordClass::IN;
    ttl_ = buffer.read_u32();
    data_len_ = buffer.read_u16();
    asio::ip::address_v4 ip(buffer.read_u32());
    data_ = AData{ip};
    break;
  }
  case RecordType::NS: {
    if (buffer.read_u16() != 1) {
      LOG(WARNING) << "Invalid record class";
      return false;
    }
    rclass_ = RecordClass::IN;
    ttl_ = buffer.read_u32();
    data_len_ = buffer.read_u16();
    if (auto v = buffer.read_qname()) {
      data_ = NSData{*v};
    } else {
      LOG(WARNING) << "Invalid qname";
      return false;
    }
    break;
  }
  case RecordType::CNAME: {
    if (buffer.read_u16() != 1) {
      LOG(WARNING) << "Invalid record class";
      return false;
    }
    rclass_ = RecordClass::IN;
    ttl_ = buffer.read_u32();
    data_len_ = buffer.read_u16();
    if (auto v = buffer.read_qname()) {
      data_ = CNAMEData{*v};
    } else {
      LOG(WARNING) << "Invalid qname";
      return false;
    }
    break;
  }
  case RecordType::MX: {
    if (buffer.read_u16() != 1) {
      LOG(WARNING) << "Invalid record class";
      return false;
    }
    rclass_ = RecordClass::IN;
    ttl_ = buffer.read_u32();
    data_len_ = buffer.read_u16();
    auto priority = buffer.read_u16();
    if (auto v = buffer.read_qname()) {
      data_ = MXData{priority, *v};
    } else {
      LOG(WARNING) << "Invalid qname";
    }
    break;
  }
  case RecordType::AAAA: {
    if (buffer.read_u16() != 1) {
      LOG(WARNING) << "Invalid record class";
    }
    rclass_ = RecordClass::IN;
    ttl_ = buffer.read_u32();
    data_len_ = buffer.read_u16();
    asio::ip::address_v4 ip(buffer.read_u32());
    data_ = AAAAData{ip};
    break;
  }
  case RecordType::OPT: {
    LOG(WARNING) << "Unhandled record type encountered, dropping";
    return false;
    break;
  }
  case RecordType::UNKNOWN: {
    buffer.step(data_len_);
    break;
  }
  default: {
    LOG(WARNING) << "No matching case for record type ";
    return false;
  }
  }
  return true;
}

bool DnsRecord::write(BytePacketBuffer &buffer) const {
  buffer.write_qname(domain_);
  buffer.write_u16(static_cast<int>(rtype_));
  buffer.write_u16(static_cast<uint16_t>(RecordClass::IN));
  buffer.write_u32(ttl_);
  if (std::holds_alternative<std::monostate>(data_)) {
    return false;
  }
  switch (rtype_) {
  case RecordType::A: {
    // write length of data
    buffer.write_u16(4);
    auto d = std::get<AData>(data_);
    auto bytes = d.ip.to_bytes();
    for (auto b : bytes) {
      buffer.write(b);
    }
    break;
  }
  case RecordType::NS: {
    // Set placeholder length
    auto pos = buffer.get_pos();
    buffer.write_u16(0);

    auto d = std::get<NSData>(data_);
    buffer.write_qname(d.host);
    auto size = buffer.get_pos() - (pos + 2);
    buffer.set_u16(pos, static_cast<uint16_t>(size));
    break;
  }
  case RecordType::CNAME: {
    // Set placeholder length
    auto pos = buffer.get_pos();
    buffer.write_u16(0);

    auto d = std::get<CNAMEData>(data_);
    buffer.write_qname(d.host);
    auto size = buffer.get_pos() - (pos + 2);
    buffer.set_u16(pos, static_cast<uint16_t>(size));
    break;
  }
  case RecordType::MX: {
    // Set placeholder length
    auto pos = buffer.get_pos();
    buffer.write_u16(0);

    auto d = std::get<MXData>(data_);
    buffer.write_u16(d.priority);
    buffer.write_qname(d.host);
    auto size = buffer.get_pos() - (pos + 2);
    buffer.set_u16(pos, static_cast<uint16_t>(size));
    break;
  }
  case RecordType::AAAA: {
    buffer.write_u16(4);
    auto d = std::get<AAAAData>(data_);
    auto bytes = d.ip.to_bytes();
    for (auto b : bytes) {
      buffer.write(b);
    }
    break;
  }
  default: {
    LOG(FATAL) << "No matching case for record type";
    break;
  }
  }
  return true;
}

std::ostream &operator<<(std::ostream &os, const DnsRecord::AData &payload) {
  os << "["
     << " ip: " << payload.ip.to_string() << "]";
  return os;
}

std::ostream &operator<<(std::ostream &os, const DnsRecord::NSData &payload) {
  os << "["
     << " host: " << payload.host << "]";
  return os;
}

std::ostream &operator<<(std::ostream &os,
                         const DnsRecord::CNAMEData &payload) {
  os << "["
     << " host: " << payload.host << "]";
  return os;
}

std::ostream &operator<<(std::ostream &os, const DnsRecord::MXData &payload) {
  os << "["
     << " priority: " << payload.priority << " host: " << payload.host << "]";
  return os;
}

std::ostream &operator<<(std::ostream &os, const DnsRecord::AAAAData &payload) {
  os << "["
     << " ip: " << payload.ip.to_string() << "]";
  return os;
}

std::ostream &operator<<(std::ostream &os, std::monostate m) {
  os << "empty variant";
  return os;
}

std::ostream &operator<<(std::ostream &os, const DnsRecord &record) {
  os << "["
     << " domain_: " << record.domain_
     << " rtype_: " << static_cast<int>(record.rtype_)
     << " rclass_: " << static_cast<int>(record.rclass_)
     << " ttl_: " << record.ttl_ << " data_len_: " << record.data_len_
     << " data_: ";
  std::visit([&os](auto &d) { os << d; }, record.data_);
  os << "]";
  return os;
}
