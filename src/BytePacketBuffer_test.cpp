#include "dns/BytePacketBuffer.hpp"

#include <gtest/gtest.h>

#include <cstdint>
class BytePacketBufferTest : public ::testing::Test {
 private:
  std::array<uint8_t, DNSPACKETSIZE> buffer;
  std::array<uint8_t, DNSPACKETSIZE> buffer1;
  std::array<uint8_t, DNSPACKETSIZE> buffer2;

 protected:
  BytePacketBufferTest()
      : buffer(), buffer1(), bpb(buffer), bpb1(buffer1), bpb2(buffer2) {
    bpb.buffer_[0] = 0x4F;
    bpb.buffer_[1] = 0x83;
    bpb.buffer_[2] = 0x89;
    bpb.buffer_[3] = 0xA3;
    bpb.buffer_[4] = 0xFF;
    bpb.buffer_[5] = 0xA3;
    bpb.buffer_[6] = 0xB3;

    bpb1.buffer_[0] = 0x06;
    bpb1.buffer_[1] = 0x67;   // g
    bpb1.buffer_[2] = 0x6f;   // o
    bpb1.buffer_[3] = 0x6f;   // o
    bpb1.buffer_[4] = 0x67;   // g
    bpb1.buffer_[5] = 0x6c;   // l
    bpb1.buffer_[6] = 0x65;   // e
    bpb1.buffer_[7] = 0x03;   //
    bpb1.buffer_[8] = 0x63;   // c
    bpb1.buffer_[9] = 0x6f;   // o
    bpb1.buffer_[10] = 0x6d;  // m
    bpb1.buffer_[11] = 0x00;
  }

  BytePacketBuffer bpb;
  BytePacketBuffer bpb1;
  BytePacketBuffer bpb2;
};

TEST_F(BytePacketBufferTest, StepTest) {
  bpb.step(2);
  EXPECT_EQ(bpb.get_pos(), 2);
  EXPECT_THROW(bpb.step(DNSPACKETSIZE + 1), std::range_error);
  EXPECT_EQ(bpb.get_pos(), 2);
  EXPECT_THROW(bpb.step(-10), std::range_error);
}

TEST_F(BytePacketBufferTest, SeekTest) {
  bpb.seek(2);
  EXPECT_EQ(bpb.get_pos(), 2);
  EXPECT_THROW(bpb.seek(DNSPACKETSIZE + 1), std::range_error);
  EXPECT_EQ(bpb.get_pos(), 2);
  EXPECT_THROW(bpb.seek(-1), std::range_error);
}

TEST_F(BytePacketBufferTest, ReadTest) {
  EXPECT_EQ(bpb.read(), 0x4F);
  EXPECT_EQ(bpb.get_pos(), 1);
  EXPECT_EQ(bpb.read_u16(), 0x8389);
  EXPECT_EQ(bpb.get_pos(), 3);
  EXPECT_EQ(bpb.read_u32(), 0xA3FFA3B3);
  EXPECT_EQ(bpb.get_pos(), 7);
}

TEST_F(BytePacketBufferTest, ReadQNameTest) {
  auto v = bpb1.read_qname();
  ASSERT_EQ(true, v.has_value());
  EXPECT_EQ("google.com", *v);
}

TEST_F(BytePacketBufferTest, WriteQNameTest) {
  bpb2.write_qname("google.com");
  EXPECT_EQ(bpb2.buffer_[0], bpb1.buffer_[0]);
  EXPECT_EQ(bpb2.buffer_[1], bpb1.buffer_[1]);
  EXPECT_EQ(bpb2.buffer_[2], bpb1.buffer_[2]);
  EXPECT_EQ(bpb2.buffer_[3], bpb1.buffer_[3]);
  EXPECT_EQ(bpb2.buffer_[4], bpb1.buffer_[4]);
  EXPECT_EQ(bpb2.buffer_[5], bpb1.buffer_[5]);
  EXPECT_EQ(bpb2.buffer_[6], bpb1.buffer_[6]);
  EXPECT_EQ(bpb2.buffer_[7], bpb1.buffer_[7]);
  EXPECT_EQ(bpb2.buffer_[8], bpb1.buffer_[8]);
  EXPECT_EQ(bpb2.buffer_[9], bpb1.buffer_[9]);
  EXPECT_EQ(bpb2.buffer_[10], bpb1.buffer_[10]);
  EXPECT_EQ(bpb2.buffer_[11], bpb1.buffer_[11]);
}
