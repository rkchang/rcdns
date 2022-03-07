#include "Server.hpp"

#include <glog/logging.h>

#include <iostream>
#include <stdexcept>
#include <variant>

#include "dns/BytePacketBuffer.hpp"
#include "dns/DnsPacket.hpp"
#include "dns/DnsRecord.hpp"

Server::Server(asio::io_context& io, int port, asio::ip::address_v4& ns_addr)
    : socket_(io, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)),
      ns_addr_(ns_addr),
      recv_buffer_() {
  receive();
}

void Server::receive() {
  while (true) {
    handle_query();
  }
}

std::optional<DnsPacket> Server::lookup(std::string& qname, RecordType qtype,
                                        asio::ip::address_v4 server_addr) {
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
  asio::ip::udp::endpoint endpoint(server_addr, 53);
  socket_.send_to(asio::buffer(bpb.buffer_), endpoint);

  std::array<uint8_t, 512> recv_buffer{};
  asio::ip::udp::endpoint sender_endpoint;
  auto len = socket_.receive_from(asio::buffer(recv_buffer), sender_endpoint);
  DLOG(INFO) << "Packet of len " << len << "received";
  BytePacketBuffer recv_bpb{recv_buffer};
  DnsPacket received_packet{};
  if (!received_packet.from_buffer(recv_bpb)) {
    DLOG(INFO) << "Invalid buffer";
    return {};
  }
  DLOG(INFO) << "Packet Decoded";

  DLOG(INFO) << "received_packet: " << received_packet;

  return received_packet;
}

std::optional<DnsPacket> Server::recursive_lookup(std::string& qname,
                                                  RecordType qtype) {
  for (;;) {
    DLOG(INFO) << "Attempting lookup of: " << qname
               << "with ns:" << ns_addr_.to_string();
    DnsPacket root_packet;
    if (auto v = lookup(qname, qtype, ns_addr_)) {
      root_packet = *v;
    } else {
      return {};
    }
  }
}

void Server::handle_query() {
  std::array<uint8_t, 512> recv_buffer{};
  asio::ip::udp::endpoint sender_endpoint;
  socket_.receive_from(asio::buffer(recv_buffer), sender_endpoint);
  BytePacketBuffer bpb{recv_buffer};
  DnsPacket request{};
  if (!request.from_buffer(bpb)) {
    DLOG(INFO) << "Invalid query, dropping";
    return;
  }

  DLOG(INFO) << "request: " << request;

  DnsPacket response{};
  response.header_.id_ = request.header_.id_;
  response.header_.recursion_desired_ = true;
  response.header_.recursion_available_ = true;
  response.header_.response_ = true;

  if (!request.questions_.empty()) {
    DLOG(INFO) << "Query received";
    auto& question = request.questions_[0];
    if (auto v = recursive_lookup(question.name_, question.rtype_)) {
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

  std::vector<uint8_t> ans{
      response_buffer.buffer_.begin(),
      response_buffer.buffer_.begin() + response_buffer.get_pos()};
  socket_.send_to(asio::buffer(ans), sender_endpoint);
}
