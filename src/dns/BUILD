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
        "@abseil-cpp//absl/log",
    ],
    copts = [
    "-Wpedantic",
    "-Wall",
    "-Wextra",
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
        "@abseil-cpp//absl/log",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
    copts = [
    "-Wpedantic",
    "-Wall",
    "-Wextra",
    ],
)
