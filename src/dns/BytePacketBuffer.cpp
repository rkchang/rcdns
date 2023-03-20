#include "BytePacketBuffer.hpp"

#include <glog/logging.h>

#include <stdexcept>
#include <string_view>
#include <vector>

BytePacketBuffer::BytePacketBuffer(std::array<uint8_t, DNSPACKETSIZE> &packet)
    : pos{0}, buffer_(packet) {}

int BytePacketBuffer::get_pos() const { return pos; }

void BytePacketBuffer::step(int steps) {
  if (pos + steps < 0 || pos + steps >= DNSPACKETSIZE) {
    throw std::range_error("Destination out of range");
  }
  pos += steps;
}

void BytePacketBuffer::seek(int dest) {
  if (dest < 0 || dest > DNSPACKETSIZE) {
    throw std::range_error("Destination out of range");
  }
  pos = dest;
}

uint8_t BytePacketBuffer::read() {
  uint8_t val = buffer_.at(pos);
  pos++;
  return val;
}

uint16_t BytePacketBuffer::get(int dest) { return buffer_.at(dest); }

/// Read two bytes, stepping two steps forward
uint16_t BytePacketBuffer::read_u16() {
  uint16_t res = this->read();
  res <<= 8;
  res |= this->read();
  return res;
}

/// Read four bytes, stepping four steps forward
uint32_t BytePacketBuffer::read_u32() {
  uint32_t res = this->read_u16();
  res <<= 16;
  res |= this->read_u16();
  return res;
}

// Read a qname
std::optional<std::string> BytePacketBuffer::read_qname() {
  int cur_pos = pos;
  bool jumped = false;
  const int max_jumps = 5;
  int jumps_performed = 0;
  std::string qname;
  std::string delim;

  while (true) {
    // Handle jump loops
    if (jumps_performed > max_jumps) {
      LOG(WARNING) << "Jump loop detected";
      return {};
    }
    uint16_t len = get(cur_pos);

    // Check for jump
    if ((len & 0xC0) == 0xC0) {
      // Move past label
      if (!jumped) {
        seek(cur_pos + 2);
      }

      // Calculate and jump to new position
      uint16_t b2 = get(cur_pos + 1);
      uint16_t offset = ((len ^ 0xC0) << 8) | b2;
      cur_pos = offset;

      jumped = true;
      jumps_performed += 1;
      continue;
    }
    cur_pos += 1;
    if (len == 0) {
      break;
    }

    // Append new label
    qname += delim;
    for (auto i = cur_pos; i < cur_pos + len; i++) {
      qname += static_cast<char>(buffer_.at(i));
    }
    delim = ".";
    cur_pos += len;
  }
  if (!jumped) {
    seek(cur_pos);
  }
  return qname;
}

void BytePacketBuffer::write(uint8_t val) {
  buffer_.at(pos) = val;
  step(1);
}

void BytePacketBuffer::write_u16(uint16_t val) {
  write(val >> 8);
  write(val & 0xFFU);
}

void BytePacketBuffer::write_u32(uint32_t val) {
  write_u16(val >> 16);
  write_u16(val & 0xFFFFU);
}

void BytePacketBuffer::write_qname(const std::string_view &qname) {
  size_t start = 0;
  size_t end = 0;
  // Iterate over labels
  while (end != qname.size()) {
    end = qname.find('.', end + 1);
    if (end == std::string_view::npos) {
      end = qname.size();
    }
    auto label = qname.substr(start, end);
    if (label.size() > 0x3F) {
      throw std::runtime_error("Label too long");
    }
    write(static_cast<uint8_t>(label.size()));
    for (const auto &c : label) {
      write(static_cast<uint8_t>(c));
    }
    start = end + 1;
  }
  write(0);
}

void BytePacketBuffer::set(int pos, uint8_t val) { buffer_.at(pos) = val; }

void BytePacketBuffer::set_u16(int pos, uint16_t val) {
  set(pos, val >> 8);
  set(pos, val & 0xFF);
}

std::ostream &operator<<(std::ostream &os, const BytePacketBuffer &bpb) {
  os << "[";
  for (std::size_t i = 0; i < bpb.buffer_.size(); i++) {
    os << bpb.buffer_[i];
    if (i != bpb.buffer_.size() - 1) {
      os << ",";
    }
  }
  os << "]";
  return os;
}