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
        "@abseil-cpp//absl/log",
        "@abseil-cpp//absl/flags:flag",
        "@abseil-cpp//absl/flags:parse",
        "@abseil-cpp//absl/strings",
    ],
    copts = [
    "-Wpedantic",
    "-Wall",
    "-Wextra",
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
        "@abseil-cpp//absl/log",
        "@abseil-cpp//absl/flags:flag",
        "@abseil-cpp//absl/flags:parse",
        "@abseil-cpp//absl/strings",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
    copts = [
    "-Wpedantic",
    "-Wall",
    "-Wextra",
    ],
)
