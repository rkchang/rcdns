#include "Server.hpp"

#include <spdlog/fmt/bundled/ranges.h>
#include <spdlog/spdlog.h>

#include <iostream>
#include <stdexcept>
#include <variant>

#include "dns/BytePacketBuffer.hpp"
#include "dns/DnsPacket.hpp"
#include "dns/DnsRecord.hpp"

Server::Server(asio::io_context& io, int port)
    : socket_(io, udp::endpoint(udp::v4(), port)), recv_buffer_() {
  receive();
}
void Server::receive() {
  socket_.async_receive_from(
      asio::buffer(recv_buffer_), remote_endpoint_,
      [this](std::error_code ec, std::size_t bytes_recvd) {
        spdlog::info("Message received: ec: {}, size: {}", ec.message(),
                     bytes_recvd);
        spdlog::info("Message: {}", recv_buffer_);
        if (!ec && bytes_recvd > 0) {
          try {
            BytePacketBuffer buffer{recv_buffer_};
            DnsPacket packet{buffer};
          } catch (std::exception& e) {
            spdlog::info("Invalid packet {}", e.what());
          }
          respond();
        } else {
          spdlog::info("Invalid message");
          receive();
        }
      });
}

void Server::respond() {
  socket_.async_send_to(asio::buffer(recv_buffer_), remote_endpoint_,
                        [this](std::error_code ec, std::size_t bytes_sent) {
                          spdlog::info("Message sent: ec: {}, size: {}",
                                       ec.message(), bytes_sent);
                          receive();
                        });
}
