cc_binary(
    name = "rcdns",
    srcs = glob(
        [
            "*.cpp",
            "*.hpp",
        ],
        exclude = ["*_test.cpp"],
    ),
    deps = [
        "//src/dns:dns_representation",
    ],
)
