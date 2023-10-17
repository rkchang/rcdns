# rcdns

This is a work in progress DNS server I wrote to learn more about DNS. It's
written in C++17 and targets Ubuntu 22.04.

Demo with opt build mode:

```
~/dev/rcdns main
❯ ./bazel-bin/src/rcdns&
[1] 492513
I20230924 01:39:02.572237 492513 main.cpp:89] Starting server with port: 54532 dns_address: 8.8.8.8 ns_address: 198.41.0.4
❯ dig @127.0.0.1 -p 54532 +noedns www.google.com

; <<>> DiG 9.18.12-0ubuntu0.22.04.3-Ubuntu <<>> @127.0.0.1 -p 54532 +noedns www.google.com
; (1 server found)
;; global options: +cmd
;; Got answer:
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 30446
;; flags: qr rd ra; QUERY: 1, ANSWER: 1, AUTHORITY: 0, ADDITIONAL: 0
;; WARNING: Message has 450 extra bytes at end

;; QUESTION SECTION:
;www.google.com.			IN	A

;; ANSWER SECTION:
www.google.com.		8	IN	A	142.251.32.68

;; Query time: 7 msec
;; SERVER: 127.0.0.1#54532(127.0.0.1) (UDP)
;; WHEN: Sun Sep 24 01:39:22 EDT 2023
;; MSG SIZE  rcvd: 512

```

# Build

## Build Dependencies

```
g++-9.3.0 or clang++-10
ninja or GNU Make
clang-tidy
ccache
bazel
lld
```

## Libraries

```
ASIO (non-Boost)
googletest
glog
abseil
```

## Building the project


### Bazel

Build optimized binary:

```
bazel build  --compilation_mode=opt --features=asan --features=ubsan //src:rcdns
```

Build debug binary:

```
bazel build  --compilation_mode=dbg --features=asan --features=ubsan //src:rcdns
```

Build tests:

```
bazel build  --compilation_mode=dbg --features=asan --features=ubsan //src:all_tests
```

# Running the project

The following are examples of running artifacts produced by debug builds of the
project

## Running the project

```
./bazel-bin/src/rcdns
OR
bazel run  --compilation_mode=dbg --features=asan --features=ubsan //src:rcdns
```

## Running tests

Tests can be run by executing the `all_tests` binary in the build directory.

```
./bazel-bin/src/all_tests
OR
bazel run  --compilation_mode=dbg --features=asan --features=ubsan //src:all_tests
```

## Compile commands

`bazel run @hedron_compile_commands//:refresh_all`

# References

- Emil Hernvall's guide to writing a DNS server was a key part in writing this project. Please visit: https://github.com/EmilHernvall/dnsguide.
