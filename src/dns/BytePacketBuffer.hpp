#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
constexpr int DNSPACKETSIZE = 512;

class BytePacketBuffer {
private:
  int pos;

public:
  // TODO(rkchang) :
  // Might be better as a unique ptr in the future
  std::array<uint8_t, DNSPACKETSIZE> buffer_;
  explicit BytePacketBuffer(std::array<uint8_t, DNSPACKETSIZE> &packet);

  int get_pos() const;
  void step(int steps);
  void seek(int dest);
  uint8_t read();
  uint16_t get(int dest);
  uint16_t read_u16();
  uint32_t read_u32();
  std::optional<std::string> read_qname();
  void write(uint8_t val);
  void write_u16(uint16_t val);
  void write_u32(uint32_t val);
  void write_qname(const std::string_view &qname);
  void set(int pos, uint8_t val);
  void set_u16(int pos, uint16_t val);
};
