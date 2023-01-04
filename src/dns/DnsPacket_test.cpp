#include "DnsPacket.hpp"

#include <gtest/gtest.h>

#include <cstdint>

#include "DnsQuestion.hpp"
#include "DnsRecord.hpp"
#include "RecordClass.hpp"
#include "RecordType.hpp"

TEST(DnsPacketTest, WriteTest) {
  DnsPacket packet{};
  packet.header_.id_ = 6666;
  packet.header_.recursion_desired_ = true;
  packet.header_.questions_ = 1;
  DnsQuestion question{};
  question.name_ = "google.com";
  question.rtype_ = RecordType::A;
  question.rclass_ = RecordClass::IN;
  packet.questions_.push_back(question);
  std::array<uint8_t, 512> arr{};
  BytePacketBuffer bpb{arr};
  packet.write(bpb);
  bpb.seek(0);

  EXPECT_EQ(6666, bpb.read_u16()); // id
  EXPECT_EQ(1U, bpb.read());       // flags_a
  EXPECT_EQ(0U, bpb.read());       // flags_b
  EXPECT_EQ(1U, bpb.read_u16());   // question count
  EXPECT_EQ(0U, bpb.read_u16());
  EXPECT_EQ(0U, bpb.read_u16());
  EXPECT_EQ(0U, bpb.read_u16());
  DnsQuestion read_question{bpb};
  EXPECT_EQ(read_question.name_, question.name_);
  EXPECT_EQ(read_question.rtype_, question.rtype_);
  EXPECT_EQ(read_question.rclass_, question.rclass_);
}
