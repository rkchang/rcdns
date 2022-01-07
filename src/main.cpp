#include <spdlog/fmt/bundled/ranges.h>
#include <spdlog/spdlog.h>

#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "Server.hpp"
#include "dns/BytePacketBuffer.hpp"
#include "dns/DnsPacket.hpp"

std::optional<DnsPacket> lookup(std::string& qname, RecordType qtype,
                                udp::socket& socket,
                                udp::endpoint& server_endpoint) {
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

  socket.send_to(asio::buffer(bpb.buffer_), server_endpoint);

  std::array<uint8_t, 512> recv_buffer{};
  udp::endpoint sender_endpoint;
  auto len = socket.receive_from(asio::buffer(recv_buffer), sender_endpoint);
  spdlog::info("Packet of len {} received", len);
  BytePacketBuffer recv_bpb{recv_buffer};
  DnsPacket received_packet{recv_bpb};
  spdlog::info("packet decoded");
  return received_packet;
}

void handle_query(udp::socket& socket, udp::endpoint& server_endpoint) {
  std::array<uint8_t, 512> recv_buffer{};
  udp::endpoint sender_endpoint;
  socket.receive_from(asio::buffer(recv_buffer), sender_endpoint);
  BytePacketBuffer bpb{recv_buffer};
  DnsPacket request{bpb};
  DnsPacket response{};
  response.header_.id_ = request.header_.id_;
  response.header_.recursion_desired_ = true;
  response.header_.recursion_available_ = true;
  response.header_.response_ = true;

  if (!request.questions_.empty()) {
    spdlog::info("Query received");
    auto& question = request.questions_[0];
    if (auto v =
            lookup(question.name_, question.rtype_, socket, server_endpoint)) {
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
  socket.send_to(asio::buffer(response_buffer.buffer_), sender_endpoint);
}

// int main() {
int main(int argc, char* argv[]) {
  spdlog::set_level(spdlog::level::debug);
  // TODO(rkchang) : Use proper library
  std::vector<std::string> args(argv, argv + argc);
  spdlog::debug("args {}", args);
  if (args.size() < 2) {
    spdlog::info("No port provided");
    return -1;
  }
  int port = std::stoi(args[1]);
  asio::io_context io;
  udp::socket socket(io, udp::endpoint(udp::v4(), port));
  udp::endpoint server_endpoint(asio::ip::make_address_v4("8.8.8.8"), 53);
  for (;;) {
    handle_query(socket, server_endpoint);
  }
  // try {
  //   Server server(io_context, port);
  //   io_context.run();
  // } catch (std::exception& e) {
  //   std::cerr << e.what() << std::endl;
  // }
}
