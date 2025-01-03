FROM ubuntu:22.04

RUN apt update && apt upgrade -y
# Get manpages and various packages back
RUN yes | /usr/local/sbin/unminimize
RUN apt install -y \
    ccache \
    clang \
    clang-format \
    clang-tidy \
    cppcheck \
    curl \
    doxygen \
    gcc \
    git \
    make \
    lld \
    ninja-build \
    python3 \
    python3-pip \
    tar \
    unzip \
    neovim \
    ripgrep \
    fish \
    tmux \
    gdb \
    neovim
# Install golang
RUN curl -LO https://go.dev/dl/go1.23.4.linux-arm64.tar.gz
RUN rm -rf /usr/local/go && tar -C /usr/local -xzf go1.23.4.linux-arm64.tar.gz
# Install bazel
RUN PATH=$PATH:/usr/local/go/bin go install github.com/bazelbuild/bazelisk@latest
RUN echo "fish_add_path /root/go/bin" >> /etc/fish/config.fish
RUN echo "alias bazel='bazelisk'" >> /etc/fish/config.fish