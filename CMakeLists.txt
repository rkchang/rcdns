cmake_minimum_required(VERSION 3.10)

# Use Ccache
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

project(rcdns VERSION 1.0)

include(src/CMakeLists.txt)

# -------------------------------
# Add libraries
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

# spdlog
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog
  GIT_TAG v1.9.2
)

FetchContent_MakeAvailable(googletest spdlog)

#find_package(spdlog REQUIRED)


# -------------------------------
# Linters and static analyzers 
# -------------------------------
set(CMAKE_CXX_CLANG_TIDY clang-tidy)
# set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE iwyu)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set ASAN options
set(ENV{ASAN_OPTIONS} "strict_string_checks=1:detect_stack_use_after_return=1:check_initialization_order=1:strict_init_order=1:detect_invalid_pointer_pairs=2")

# Run Sanitizers for debug builds
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} \
  -fsanitize=address \
  -fsanitize=leak \
  -fno-omit-frame-pointer \
  -fsanitize=undefined \
  -fsanitize=float-divide-by-zero \
  -fsanitize=float-cast-overflow"
)
set (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,undefined")
add_executable(rcdns 
  ${SRC_FILES}
)

# -------------------------------
# Compile flags 
# -------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

target_compile_options(rcdns 
  PRIVATE
  -Walloca 
  -Wcast-qual 
  -Wconversion 
  -Wformat=2 
  -Wformat-security 
  -Wnull-dereference 
  -Wstack-protector 
  -Wstrict-overflow=3 
  -Wvla 
  -Warray-bounds 
  -Wfloat-equal 
  -Wimplicit-fallthrough 
  -Wpointer-arith 
  -Wswitch-enum 
  -D_FORTIFY_SOURCE=2
  -fstack-protector-strong 
  -fPIE
)
target_link_libraries(rcdns
  PRIVATE 
  gtest_main 
  spdlog::spdlog
)

# -------------------------------
# Google Test 
# -------------------------------

enable_testing()

add_executable(
  hello_test
  ${TEST_FILES}
)
target_link_libraries(
  hello_test
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)
