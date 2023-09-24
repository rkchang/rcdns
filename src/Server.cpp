#include "Server.hpp"
#include "Constants.hpp"

#include <glog/logging.h>

#include <iostream>
#include <stdexcept>
#include <variant>

#include "dns/BytePacketBuffer.hpp"
#include "dns/DnsPacket.hpp"
#include "dns/DnsRecord.hpp"

Server::Server(asio::io_context &io, int port, std::string &dns_address,
               std::string &ns_address)
    : socket_(io, udp::endpoint(udp::v4(), port)),
      ns_endpoint_(asio::ip::make_address_v4(ns_address),
                   constants::DNS_PORT_NUM),
      dns_endpoint_(asio::ip::make_address_v4(dns_address),
                    constants::DNS_PORT_NUM),
      ns_address_(ns_address), recv_buffer_() {
  receive();
}

void Server::receive() {
  while (true) {
    std::array<uint8_t, 512> recv_buffer{};
    udp::endpoint sender_endpoint;
    socket_.receive_from(asio::buffer(recv_buffer), sender_endpoint);
    BytePacketBuffer bpb{recv_buffer};
    DnsPacket recvd{bpb};
    DLOG(INFO) << "incoming from: " << sender_endpoint.address();
    if (recvd.header_.rescode_ != DnsHeader::ResultCode::NOERROR) {
      LOG(INFO) << "DnsPacket Error" << recvd;
    }
    if (!recvd.authorities_.empty() &&
        recvd.header_.rescode_ == DnsHeader::ResultCode::NOERROR) {
      // If authorities isn't empty then this is for a previous query
      DLOG(INFO) << "Authority received " << recvd;
      handle_authority(recvd);
    } else if (!recvd.answers_.empty() &&
               recvd.header_.rescode_ == DnsHeader::ResultCode::NOERROR) {
      // If answers isn't empty then a previous query is complete and this is
      // the final recursive resolve
      DLOG(INFO) << "Answers received " << recvd;
      handle_answer(recvd);
    } else if (!recvd.questions_.empty()) {
      // If this is a question but no authorities then this is a new query
      DLOG(INFO) << "Query received " << recvd;
      for (const auto &q : recvd.questions_) {
        queries_.insert({q.name_, {recvd.header_.id_, sender_endpoint}});
        lookup(recvd.header_.id_, q.name_, q.rtype_, dns_endpoint_);
      }
    } else {
      DLOG(INFO) << "Dropping unsupported packet " << recvd;
      std::cout << recvd << std::endl;
    }
  }
}

void Server::lookup(const uint16_t header_id, const std::string &qname,
                    RecordType qtype, const udp::endpoint &server_endpoint) {
  DnsPacket packet{};
  packet.header_.id_ = header_id;
  packet.header_.recursion_desired_ = true;
  packet.header_.questions_ = 1;
  DnsQuestion question{qname, qtype, RecordClass::IN};
  packet.questions_.push_back(question);
  std::array<uint8_t, 512> arr{};
  BytePacketBuffer bpb{arr};
  packet.write(bpb);

  DLOG(INFO) << "lookup packet to: " << server_endpoint.address() << " "
             << packet;
  socket_.send_to(asio::buffer(bpb.buffer_), server_endpoint);
}

void Server::handle_authority(const DnsPacket &recvd) {
  if (recvd.questions_.size() <= 0) {
    // must have a question
    // TODO: Send error to queryer?
    // TODO: Could end up with a query that never leaves
    return;
  }
  auto &first_question = recvd.questions_[0];
  auto &first_authority = recvd.authorities_[0];
  auto &host = std::get<DnsRecord::NSData>(first_authority.data_).host;
  udp::endpoint auth_endpoint(asio::ip::make_address_v4(host),
                              constants::DNS_PORT_NUM);

  lookup(recvd.header_.id_, first_question.name_, first_question.rtype_,
         auth_endpoint);
}

void Server::handle_answer(const DnsPacket &recvd) {
  auto &first_answer = recvd.answers_[0];
  switch (first_answer.rtype_) {
  case RecordType::A: {
    DnsPacket response{};
    response.header_.id_ = recvd.header_.id_;
    response.header_.recursion_desired_ = true;
    response.header_.recursion_available_ = true;
    response.header_.response_ = true;
    response.questions_ = recvd.questions_;
    response.answers_ = recvd.answers_;

    std::array<uint8_t, 512> buffer{};
    BytePacketBuffer response_buffer{buffer};
    response.write(response_buffer);

    std::string query_domain = recvd.questions_.at(0).name_;
    udp::endpoint endpoint = std::get<udp::endpoint>(queries_.at(query_domain));
    DLOG(INFO) << "answer to: " << endpoint.address() << " " << response;
    socket_.send_to(asio::buffer(response_buffer.buffer_), endpoint);
    queries_.erase(query_domain);
    break;
  }
  default:
    DLOG(INFO) << "Dropping unsupported answer" << recvd;
  }
}
