#pragma once

#include <array>
#include <asio.hpp>
#include <cstdint>

using asio::ip::udp;
class Server {
 public:
  Server(asio::io_context& io_context, int port);
  void receive();
  void respond();
  // TODO(rkchang): follow naming scheme for member variables
  udp::socket socket_;
  udp::endpoint remote_endpoint_;
  std::array<uint8_t, 512> recv_buffer_;
};