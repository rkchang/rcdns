cc_binary(
    name = "rcdns",
    srcs = glob(
        [
            "*.cpp",
            "*.hpp",
        ],
        exclude = glob(["*_test.cpp"]),
    ),
    defines = ["ASIO_STANDALONE"],
    deps = [
        "//src/dns:dns_representation",
        "@com_github_google_glog//:glog",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/strings",
        "@my-asio//:asio",
    ],
)
