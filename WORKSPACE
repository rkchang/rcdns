load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "com_github_google_glog",
    sha256 = "122fb6b712808ef43fbf80f75c52a21c9760683dae470154f02bddfc61135022",
    strip_prefix = "glog-0.6.0",
    urls = ["https://github.com/google/glog/archive/v0.6.0.zip"],
)

http_archive(
    name = "com_github_gflags_gflags",
    sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
    strip_prefix = "gflags-2.2.2",
    urls = ["https://github.com/gflags/gflags/archive/v2.2.2.tar.gz"],
)

http_archive(
    name = "com_google_googletest",
    sha256 = "5cf189eb6847b4f8fc603a3ffff3b0771c08eec7dd4bd961bfd45477dd13eb73",
    strip_prefix = "googletest-609281088cfefc76f9d0ce82e1ff6c30cc3591e5",
    urls = ["https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip"],
)

http_archive(
    name = "com_google_absl",
    strip_prefix = "abseil-cpp-98eb410c93ad059f9bba1bf43f5bb916fc92a5ea",
    urls = ["https://github.com/abseil/abseil-cpp/archive/98eb410c93ad059f9bba1bf43f5bb916fc92a5ea.zip"],
)

http_archive(
    name = "my-asio",
    build_file_content = """
cc_library(
    name = "asio",
    visibility = ["//visibility:public"],
    hdrs = glob(["asio/include/**/*.hpp"]) + glob(["asio/include/**/*.ipp"]),
    defines = ["ASIO_STANDALONE"],
    includes = ["asio/include"],
)
""",
    sha256 = "0167cc987f346f9ea57b394518c6ea54aa6c641460dd2cd1f82200662c67bf6a",
    strip_prefix = "asio-6c054e98f3f53352d12b6cd46d63b6d404cc044b",
    urls = ["https://github.com/chriskohlhoff/asio/archive/6c054e98f3f53352d12b6cd46d63b6d404cc044b.zip"],
)
