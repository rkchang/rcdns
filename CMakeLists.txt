cmake_minimum_required(VERSION 3.11)

# Use Ccache
# find_program(CCACHE_PROGRAM ccache)
# if(CCACHE_PROGRAM)
#     set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
# endif()

project(rcdns VERSION 1.0)

include(src/CMakeLists.txt)

set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Available Build types
set(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release Tsan)

# -------------------------------
# Libraries
# -------------------------------
include(FetchContent)

# Gtest
# Source: https://google.github.io/googletest/quickstart-cmake.html
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG release-1.11.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# glog
find_package(glog 0.5.0 REQUIRED)

# abseil
add_subdirectory(external/abseil-cpp) 

# asio
add_library(asio INTERFACE)
target_include_directories(asio SYSTEM INTERFACE external/asio/asio/include)


# -------------------------------
# Compile Flags 
# -------------------------------

# set ASAN options
set(ENV{ASAN_OPTIONS} "strict_string_checks=1:detect_stack_use_after_return=1:check_initialization_order=1:strict_init_order=1:detect_invalid_pointer_pairs=2")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(ENV{ASAN_OPTIONS} "\ 
    strict_string_checks=1:\
    detect_stack_use_after_return=1:\
    check_initialization_order=1:\
    strict_init_order=1:\
    detect_invalid_pointer_pairs=2"
  )
elseif(CMAKE_BUILD_TYPE STREQUAL "Debugger")
  set(ENV{ASAN_OPTIONS} "\ 
    detect_leaks=0:\ 
    strict_string_checks=1:\
    detect_stack_use_after_return=1:\
    check_initialization_order=1:\
    strict_init_order=1:\
    detect_invalid_pointer_pairs=2"
  )
endif()

# Debug builds
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} \
  -fsanitize=address \
  -fsanitize=leak \
  -fno-omit-frame-pointer \
  -fsanitize=undefined \
  -fsanitize=float-divide-by-zero \
  -fsanitize=float-cast-overflow \
  -fno-optimize-sibling-calls \
  -g"
)
set (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} \
  -fsanitize=address,undefined \
  -g"
)

# Debugger builds - Leak sanitizer does not run under gdb
set (CMAKE_CXX_FLAGS_DEBUGGER "${CMAKE_CXX_FLAGS_DEBUGGER} \
  -fsanitize=undefined \
  -g"
)

# Tsan builds
set (CMAKE_CXX_FLAGS_TSAN "\
  -fsanitize=thread \
  -g"
)
set (CMAKE_EXE_LINKER_FLAGS_TSAN "\
  -fsanitize=thread \
  -g"
)

add_executable(rcdns 
  ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
  ${SRC_FILES}
)

# set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE iwyu)
# set_target_properties(rcdns PROPERTIES 
#   CXX_CLANG_TIDY "clang-tidy"
# )

target_precompile_headers(rcdns PRIVATE 
  external/asio/asio/include/asio.hpp
  <optional>
  <string>
  <array>
  <vector>
)

target_compile_options(rcdns 
  PRIVATE
  -Wall 
  -Wextra 
  -fstack-protector-strong 
  -fPIE
)

target_link_options(rcdns
  PRIVATE
  -fuse-ld=lld
)

target_link_libraries(rcdns
  PRIVATE 
  asio
  glog::glog
  absl::base
  # absl::debugging
  # absl::flat_hash_map
  absl::flags
  absl::flags_parse
  # absl::memory
  # absl::meta
  # absl::numeric
  # absl::random_random
  absl::strings
  # absl::synchronization
  # absl::time
  # absl::utility
)

# -------------------------------
# Google Test 
# -------------------------------

enable_testing()

add_executable(all_tests
  ${TEST_FILES} 
  ${SRC_FILES}
  ${HPP_FILES}
)

# Disable clang-tidy for tests. google-test triggers too many warnings
# set_target_properties(all_tests PROPERTIES 
#   CXX_CLANG_TIDY ""
# )

target_link_libraries(all_tests
  gtest_main
  asio
  glog::glog
  absl::strings
  absl::base
)


include(GoogleTest)
gtest_discover_tests(all_tests)

