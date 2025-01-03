#pragma once
#include <array>
#include <boost/asio.hpp>
#include <string>

class Server {
public:
  Server(int port, std::string &src_address, std::string &ns_address);
  void run();
private:
  void receive();
  void respond();
  std::string get_addr_str(struct sockaddr_storage *);
  std::array<uint8_t, 512> recv_buffer_;
  int recv_socket_;
  int ns_socket_;
};