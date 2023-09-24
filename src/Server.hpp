#pragma once

#include <array>
#include <asio.hpp>
#include <cstdint>
#include <string>

#include "dns/DnsPacket.hpp"

using asio::ip::udp;
class Server {
private:
  void handle_authority(const DnsPacket &answer);
  void handle_answer(const DnsPacket &answer);
  void lookup(const uint16_t header_id, const std::string &qname,
              RecordType qtype, const udp::endpoint &server_endpoint);
  void send_formerr(const DnsPacket &request);

public:
  Server(asio::io_context &io_context, int port, std::string &address,
         std::string &ns_address);
  void receive();
  void respond();
  udp::socket socket_;
  udp::endpoint remote_endpoint_;
  udp::endpoint ns_endpoint_;
  udp::endpoint dns_endpoint_;
  std::string ns_address_;
  std::array<uint8_t, 512> recv_buffer_;
  // query domain -> (query header id, inquirer endpoint)
  std::unordered_map<std::string, std::pair<int, udp::endpoint>> queries_;
};