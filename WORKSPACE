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
    # sha256 = "5cf189eb6847b4f8fc603a3ffff3b0771c08eec7dd4bd961bfd45477dd13eb73",
    strip_prefix = "googletest-b796f7d44681514f58a683a3a71ff17c94edb0c1",
    urls = ["https://github.com/google/googletest/archive/b796f7d44681514f58a683a3a71ff17c94edb0c1.zip"],
)

# Required by abseil
http_archive(
  name = "bazel_skylib",
  urls = ["https://github.com/bazelbuild/bazel-skylib/releases/download/1.2.1/bazel-skylib-1.2.1.tar.gz"],
  sha256 = "f7be3474d42aae265405a592bb7da8e171919d74c16f082a5457840f06054728",
)


http_archive(
    name = "com_google_absl",
    strip_prefix = "abseil-cpp-c8a2f92586fe9b4e1aff049108f5db8064924d8e",
    urls = ["https://github.com/abseil/abseil-cpp/archive/c8a2f92586fe9b4e1aff049108f5db8064924d8e.zip"],
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

http_archive(
    name = "hedron_compile_commands",
    strip_prefix = "bazel-compile-commands-extractor-f7388651ee99608fb5f6336764657596e2f84b97",

    # Replace the commit hash in both places (below) with the latest, rather than using the stale one here.
    # Even better, set up Renovate and let it do the work for you (see "Suggestion: Updates" in the README).
    url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/f7388651ee99608fb5f6336764657596e2f84b97.tar.gz",
    # When you first run this tool, it'll recommend a sha256 hash to put here with a message like: "DEBUG: Rule 'hedron_compile_commands' indicated that a canonical reproducible form can be obtained by modifying arguments sha256 = ..."
)
load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")
hedron_compile_commands_setup()
