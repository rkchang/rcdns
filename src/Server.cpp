#include "Server.hpp"

#include <glog/logging.h>

#include <iostream>
#include <stdexcept>
#include <variant>

#include "dns/BytePacketBuffer.hpp"
#include "dns/DnsPacket.hpp"
#include "dns/DnsRecord.hpp"

Server::Server(asio::io_context& io, int port, std::string& address)
    : socket_(io, udp::endpoint(udp::v4(), port)),
      server_endpoint_(asio::ip::make_address_v4(address), 53),
      recv_buffer_() {
  receive();
}
void Server::receive() {
  while (true) {
    handle_query();
  }
}

std::optional<DnsPacket> Server::lookup(std::string& qname, RecordType qtype) {
  DnsPacket packet{};
  packet.header_.id_ = 6666;
  packet.header_.recursion_desired_ = true;
  packet.header_.questions_ = 1;
  DnsQuestion question{};
  question.name_ = qname;
  question.rtype_ = qtype;
  question.rclass_ = RecordClass::IN;
  packet.questions_.push_back(question);
  std::array<uint8_t, 512> arr{};
  BytePacketBuffer bpb{arr};
  packet.write(bpb);

  DLOG(INFO) << "packet: " << packet;
  socket_.send_to(asio::buffer(bpb.buffer_), server_endpoint_);

  std::array<uint8_t, 512> recv_buffer{};
  udp::endpoint sender_endpoint;
  auto len = socket_.receive_from(asio::buffer(recv_buffer), sender_endpoint);
  DLOG(INFO) << "Packet of len " << len << "received";
  BytePacketBuffer recv_bpb{recv_buffer};
  DnsPacket received_packet{recv_bpb};
  DLOG(INFO) << "Packet Decoded";

  DLOG(INFO) << "received_packet: " << received_packet;

  return received_packet;
}

void Server::handle_query() {
  std::array<uint8_t, 512> recv_buffer{};
  udp::endpoint sender_endpoint;
  socket_.receive_from(asio::buffer(recv_buffer), sender_endpoint);
  BytePacketBuffer bpb{recv_buffer};
  DnsPacket request{bpb};

  DLOG(INFO) << "request: " << request;

  DnsPacket response{};
  response.header_.id_ = request.header_.id_;
  response.header_.recursion_desired_ = true;
  response.header_.recursion_available_ = true;
  response.header_.response_ = true;

  if (!request.questions_.empty()) {
    DLOG(INFO) << "Query received";
    auto& question = request.questions_[0];
    if (auto v = lookup(question.name_, question.rtype_)) {
      auto& result = *v;
      response.questions_.push_back(question);
      response.header_.rescode_ = result.header_.rescode_;
      response.answers_ = result.answers_;
      response.authorities_ = result.authorities_;
      response.resources_ = result.resources_;
    } else {
      response.header_.rescode_ = DnsHeader::ResultCode::SERVFAIL;
    }
  } else {
    response.header_.rescode_ = DnsHeader::ResultCode::FORMERR;
  }
  std::array<uint8_t, 512> buffer{};
  BytePacketBuffer response_buffer{buffer};
  response.write(response_buffer);

  DLOG(INFO) << "response: " << response;

  socket_.send_to(asio::buffer(response_buffer.buffer_), sender_endpoint);
}
