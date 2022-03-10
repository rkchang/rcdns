cc_binary(
    name = "rcdns",
    srcs = glob(
        [
            "*.cpp",
            "*.hpp",
        ],
        exclude = glob(["*_test.cpp"]),
    ),
    deps = [
        "//src/dns:dns_representation",
        "@com_github_google_glog//:glog",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/strings",
        "@my-asio//:asio",
    ],
)

cc_test(
    name = "all_tests",
    srcs = glob(
        [
            "*.cpp",
            "*.hpp",
        ],
        exclude = ["main.cpp"],
    ),
    deps = [
        "//src/dns:dns_representation",
        "//src/dns:dns_representation_test",
        "@com_github_google_glog//:glog",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/strings",
        "@my-asio//:asio",
        "@com_google_googletest//:gtest_main",
    ],
)