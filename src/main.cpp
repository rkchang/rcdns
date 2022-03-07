#include <glog/logging.h>

#include <array>
#include <asio.hpp>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "Server.hpp"
#include "absl/flags/flag.h"
#include "absl/flags/marshalling.h"
#include "absl/flags/parse.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_split.h"
#include "dns/BytePacketBuffer.hpp"
#include "dns/DnsPacket.hpp"

// Taken from: https://abseil.io/docs/cpp/guides/flags
struct PortNumber {
  explicit PortNumber(int p = 0) : port(p) {}

  int port;  // Valid range is [0..32767]
};

struct Ipv4Address {
  explicit Ipv4Address(std::string a = "") : addr(a) {}

  std::string addr;
};

// // Taken from: https://abseil.io/docs/cpp/guides/flags
// // Returns a textual flag value corresponding to the PortNumber `p`.
std::string AbslUnparseFlag(PortNumber p) {
  // Delegate to the usual unparsing for int.
  return absl::UnparseFlag(p.port);
}

std::string AbslUnparseFlag(Ipv4Address a) { return absl::UnparseFlag(a.addr); }

// // Taken from: https://abseil.io/docs/cpp/guides/flags
bool AbslParseFlag(absl::string_view text, PortNumber* p, std::string* error) {
  // Convert from text to int using the int-flag parser.
  if (!absl::ParseFlag(text, &p->port, error)) {
    return false;
  }
  if (p->port < 0 || p->port > 65535) {
    *error = "not in range [0,65535]";
    return false;
  }
  return true;
}

bool AbslParseFlag(absl::string_view text, Ipv4Address* a, std::string* error) {
  // Convert from text to int using the int-flag parser.
  if (!absl::ParseFlag(text, &a->addr, error)) {
    return false;
  }
  std::vector<std::string> v = absl::StrSplit(a->addr, '.');
  if (v.size() != 4) {
    *error = "not enough octets";
    return false;
  }
  for (const auto& elem : v) {
    int32_t octet;
    if (!absl::SimpleAtoi(elem, &octet) || octet > 255 || octet < 0) {
      *error = "octet: " + elem + " not in range [0,255]";
      return false;
    }
  }
  return true;
}

// Taken from: https://abseil.io/docs/cpp/guides/flags
ABSL_FLAG(PortNumber, port, PortNumber(44532), "What port to listen on");
ABSL_FLAG(Ipv4Address, addr, Ipv4Address("8.8.8.8"), "What DNS server to use");

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);  // NOLINT
  absl::ParseCommandLine(argc, argv);
  FLAGS_logtostderr = true;
  int port = absl::GetFlag(FLAGS_port).port;
  std::string address = absl::GetFlag(FLAGS_addr).addr;
  LOG(INFO) << "Starting server with port: " << port << " address: " << address;
  asio::io_context io;
  auto asio_addr = asio::ip::make_address_v4(address);
  Server server{io, port, asio_addr};
  try {
    io.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}
