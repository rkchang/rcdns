# rcdns

This is a work in progress DNS server I wrote to learn more about DNS. It's
written in C++17 and targets Ubuntu 20.04. 

# Build 
## Build Dependencies 

``` 
g++-9.3.0 or clang++-10 
ninja or GNU Make
clang-tidy 
ccache 
cmake 
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

There is a convenience Makefile (rcdns/Makefile) provided to build the project.
The targets `debug` and `debugger` for the Makefile produce builds for
debugging and for gdb respectively. If you don't want to or can't use the
Makefile, the following commands should create a debug build of the project for
you. 

``` 
mkdir build_debug 
cd build_debug 
cmake -G Ninja -DABSL_PROPAGATE_CXX_STD=ON -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Debug .. 
ninja 
```

Project build types:
- `debug`: Compiles with Address, Leak and Undefined Behavior sanitizers
- `debugger`: For use with debugging via gdb. Includes sanitizers used in
  `debug` except for Leak sanitizer as it's not compatible

Clang-tidy is run on each build using the configuration provided in
.clang-tidy.

# Running the project

The following are examples of running artifacts produced by debug builds of the
project

## Running the project

```
./build_debug/rcdns
```

## Running tests

Tests can be run by executing the `all_tests` binary in the build directory.

```
./build_debug/all_tests
```

# References
- Emil Hernvall's guide to writing a DNS server was a key part in writing this project. Please visit: https://github.com/EmilHernvall/dnsguide.
