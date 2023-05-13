#pragma once

#include <array>
#include <asio.hpp>
#include <cstdint>
#include <string>

#include "dns/DnsPacket.hpp"

using asio::ip::udp;
class Server {
private:
  void handle_answer(const DnsPacket &query);
  void lookup(const std::string &qname, RecordType qtype, int id);
  void recursive_lookup(const std::string &qname, RecordType qtype, int id);
  void send_formerr(const DnsPacket &request);

public:
  Server(asio::io_context &io_context, int port, std::string &address,
         std::string &ns_address);
  void receive();
  void respond();
  udp::socket socket_;
  udp::endpoint remote_endpoint_;
  udp::endpoint server_endpoint_;
  std::string ns_address_;
  std::array<uint8_t, 512> recv_buffer_;
  std::unordered_map<int, udp::endpoint> questions_;
};