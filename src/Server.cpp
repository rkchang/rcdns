#include "Server.hpp"
#include "Constants.hpp"

#include <glog/logging.h>

#include <iostream>
#include <stdexcept>
#include <variant>

#include "dns/BytePacketBuffer.hpp"
#include "dns/DnsPacket.hpp"
#include "dns/DnsRecord.hpp"

Server::Server(asio::io_context &io, int port, std::string &address,
               std::string &ns_address)
    : socket_(io, udp::endpoint(udp::v4(), port)),
      server_endpoint_(asio::ip::make_address_v4(address), constants::PORT_NUM),
      ns_address_(ns_address), recv_buffer_() {
  receive();
}

void Server::receive() {
  while (true) {
    std::array<uint8_t, 512> recv_buffer{};
    udp::endpoint sender_endpoint;
    socket_.receive_from(asio::buffer(recv_buffer), sender_endpoint);
    BytePacketBuffer bpb{recv_buffer};
    DnsPacket incoming{bpb};
    DLOG(INFO) << "incoming: " << incoming;
    if (!incoming.answers_.empty()) {
      DLOG(INFO) << "Answers received";
      handle_answer(incoming);
    } else if (!incoming.questions_.empty()) {
      DLOG(INFO) << "Query received";
      auto &question = incoming.questions_[0];
      int id = incoming.header_.id_;
      questions_.insert({id, sender_endpoint});
      lookup(question.name_, question.rtype_, id);
    } else {
      std::cout << incoming << std::endl;
    }
  }
}

void Server::lookup(const std::string &qname, RecordType qtype, int id) {
  DnsPacket packet{};
  packet.header_.id_ = id;
  packet.header_.recursion_desired_ = true;
  packet.header_.questions_ = 1;
  DnsQuestion question{qname, qtype, RecordClass::IN};
  packet.questions_.push_back(question);
  std::array<uint8_t, 512> arr{};
  BytePacketBuffer bpb{arr};
  packet.write(bpb);

  DLOG(INFO) << "lookup packet: " << packet;
  socket_.send_to(asio::buffer(bpb.buffer_), server_endpoint_);

  // std::array<uint8_t, 512> recv_buffer{};
  // udp::endpoint sender_endpoint;
  // auto len = socket_.receive_from(asio::buffer(recv_buffer),
  // sender_endpoint); DLOG(INFO) << "Packet of len " << len << "received";
  // BytePacketBuffer recv_bpb{recv_buffer};
  // DnsPacket received_packet{recv_bpb};
  // DLOG(INFO) << "Packet Decoded";

  // DLOG(INFO) << "received_packet: " << received_packet;
}

void Server::recursive_lookup(const std::string &qname, RecordType qtype,
                              int id) {
  auto ns_addr = ns_address_;
  while (true) {
    DLOG(INFO) << "attempting lookup of: " << name_from_rtype(qtype) << " "
               << qname << " with ns: " << ns_address_;
    udp::endpoint endpoint(asio::ip::make_address_v4(ns_addr),
                           constants::PORT_NUM);
  }
}

void Server::handle_answer(const DnsPacket &answer) {
  DnsPacket response{};
  response.header_.id_ = answer.header_.id_;
  response.header_.recursion_desired_ = true;
  response.header_.recursion_available_ = true;
  response.header_.response_ = true;
  response.questions_ = answer.questions_;
  response.answers_ = answer.answers_;

  std::array<uint8_t, 512> buffer{};
  BytePacketBuffer response_buffer{buffer};
  response.write(response_buffer);

  udp::endpoint endpoint = questions_.at(answer.header_.id_);
  DLOG(INFO) << "answer: " << response;
  socket_.send_to(asio::buffer(response_buffer.buffer_), endpoint);
}
