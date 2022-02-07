#include <glog/logging.h>

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

// int main() {
int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  FLAGS_logtostderr = 1;

  // TODO(rkchang) : Use proper library
  std::vector<std::string> args(argv, argv + argc);
  CHECK(args.size() >= 3) << "Insufficient arguments";
  int port = std::stoi(args[1]);
  asio::io_context io;
  Server server{io, port, args[2]};
  try {
    io.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}
