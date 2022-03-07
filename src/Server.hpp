#pragma once

#include <array>
#include <asio.hpp>
#include <cstdint>
#include <string>

#include "dns/DnsPacket.hpp"

class Server {
 private:
  void handle_query();
  std::optional<DnsPacket> lookup(std::string& qname, RecordType qtype,
                                  asio::ip::address_v4 server_addr);
  std::optional<DnsPacket> recursive_lookup(std::string& qname,
                                            RecordType qtype);

 public:
  Server(asio::io_context& io_context, int port, asio::ip::address_v4& address);
  void receive();
  void respond();
  asio::ip::udp::socket socket_;
  asio::ip::udp::endpoint remote_endpoint_;
  asio::ip::address_v4 ns_addr_;
  std::array<uint8_t, 512> recv_buffer_;
};