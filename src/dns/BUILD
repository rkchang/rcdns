load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "dns_representation",
    srcs = glob(
        [
            "*.cpp",
        ],
        exclude = glob(["*_test.cpp"]),
    ),
    hdrs = glob([
        "*.hpp",
    ]),
    visibility = ["//visibility:public"],
    deps = [
        "@com_github_google_glog//:glog",
        "@my-asio//:asio",
    ],
)

cc_library(
    name = "dns_representation_test",
    srcs = glob(
        [
            "*.cpp",
        ],
    ),
    hdrs = glob([
        "*.hpp",
    ]),
    visibility = ["//visibility:public"],
    deps = [
        "@com_github_google_glog//:glog",
        "@my-asio//:asio",
        "@com_google_googletest//:gtest_main",
    ],
)
