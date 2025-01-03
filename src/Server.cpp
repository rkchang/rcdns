#include "Server.hpp"
#include "Constants.hpp"
#include "absl/log/log.h"
#include "dns/BytePacketBuffer.hpp"
#include "dns/DnsPacket.hpp"
#include <array>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
using namespace constants;

Server::Server(int port, std::string &src_address, std::string &ns_address) {
  struct addrinfo addrinfo_hints = {};
  // struct addrinfo *ns_addrinfo_res = nullptr;
  struct addrinfo *local_addrinfo_res = nullptr;
  addrinfo_hints.ai_family = AF_INET;      // IPv4
  addrinfo_hints.ai_socktype = SOCK_DGRAM; // udp
  // TODO(rkchang): check errcode and other results of res
  // getaddrinfo for the nameserver
  //   int ret = getaddrinfo(ns_address.data(), "53", &addrinfo_hints,
  //   &ns_addrinfo_res); if (ret != 0) {
  //     LOG(ERROR) << "getaddrinfo() failed: " << gai_strerror(ret);
  //   }
  // TODO(rkchang): can i reuse the getaddrinfos?
  // getaddrinfo for when we bind
  int ret = getaddrinfo(src_address.data(), std::to_string(port).data(),
                        &addrinfo_hints, &local_addrinfo_res);
  if (ret != 0) {
    LOG(ERROR) << "getaddrinfo() failed: " << gai_strerror(ret);
  }
  // Create our ns_socket_
  // ns_socket_ = socket(ns_addrinfo_res->ai_family,
  // ns_addrinfo_res->ai_socktype,
  //                     ns_addrinfo_res->ai_protocol);
  // Create our recv_socket_
  recv_socket_ =
      socket(local_addrinfo_res->ai_family, local_addrinfo_res->ai_socktype,
             local_addrinfo_res->ai_protocol);
  if (recv_socket_ == -1) {
    // TODO: List the servinfo
    DLOG(INFO) << "Socket creation failed";
  }
  // bind to our desired port and source address
  if (bind(recv_socket_, local_addrinfo_res->ai_addr,
           local_addrinfo_res->ai_addrlen) != 0) {
    LOG(WARNING) << "bind() failed";
  }
}

// TODO: make reference
std::string Server::get_addr_str(struct sockaddr_storage *storage) {
  auto *sock_addr = (struct sockaddr *)storage;
  const void *in_addr = nullptr;
  if (sock_addr->sa_family == AF_INET) {
    auto *sock_addr_in = (struct sockaddr_in *)storage;
    in_addr = &(sock_addr_in->sin_addr);
  } else {
    auto *sock_addr_in = (struct sockaddr_in6 *)storage;
    in_addr = &(sock_addr_in->sin6_addr);
  }
  std::array<char, INET6_ADDRSTRLEN> addr_buf{};
  const char *addr =
      inet_ntop(storage->ss_family, in_addr, addr_buf.data(), addr_buf.size());
  std::string saddr = addr;
  return saddr;
}

void Server::run() {
  struct sockaddr_storage their_addr = {};
  while (true) {
    std::array<uint8_t, DNS_PACKET_SIZE> recv_buffer{};
    socklen_t addr_len = sizeof their_addr;
    DLOG(INFO) << "Waiting for message";
    size_t num_bytes =
        recvfrom(recv_socket_, recv_buffer.data(), DNS_PACKET_SIZE, 0,
                 (struct sockaddr *)&their_addr, &addr_len);
    std::string addr = get_addr_str(&their_addr);
    DLOG(INFO) << "Received packet size: " << num_bytes << " from: " << addr;
    BytePacketBuffer bpb{recv_buffer};
    DnsPacket recvd{bpb};
  }
}
