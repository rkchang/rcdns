FROM ubuntu:21.10

ARG DEBIAN_FRONTEND=noninteractive
RUN apt update && apt upgrade -y
RUN apt install -y \
    ccache \
    clang \
    clang-format \
    clang-tidy \
    cmake \
    cppcheck \
    curl \
    doxygen \
    gcc \
    git \
    make \
    ninja-build \
    python3 \
    python3-pip \
    tar \
    unzip \
    neovim \
    fish
