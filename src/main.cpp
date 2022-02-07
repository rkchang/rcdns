#include <glog/logging.h>
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

// int main() {
int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  FLAGS_logtostderr = 1;

  spdlog::set_level(spdlog::level::debug);
  // TODO(rkchang) : Use proper library
  std::vector<std::string> args(argv, argv + argc);
  spdlog::debug("args {}", args);
  if (args.size() < 3) {
    spdlog::info("Insufficient arguements");
    return -1;
  }
  int port = std::stoi(args[1]);
  asio::io_context io;
  Server server{io, port, args[2]};
  try {
    io.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}
