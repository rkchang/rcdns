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
    strip_prefix = "googletest-58d77fa8070e8cec2dc1ed015d66b454c8d78850",
    urls = ["https://github.com/google/googletest/archive/58d77fa8070e8cec2dc1ed015d66b454c8d78850.zip"],
)

# Required by abseil
http_archive(
    name = "bazel_skylib",
    sha256 = "f7be3474d42aae265405a592bb7da8e171919d74c16f082a5457840f06054728",
    urls = ["https://github.com/bazelbuild/bazel-skylib/releases/download/1.2.1/bazel-skylib-1.2.1.tar.gz"],
)

http_archive(
    name = "com_google_absl",
    strip_prefix = "abseil-cpp-8c0b94e793a66495e0b1f34a5eb26bd7dc672db0",
    urls = ["https://github.com/abseil/abseil-cpp/archive/8c0b94e793a66495e0b1f34a5eb26bd7dc672db0.zip"],
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
