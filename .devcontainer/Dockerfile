FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install core tools
RUN apt-get update && apt-get install -y \
    build-essential \
    qemu-system-x86 \
    git \
    gdb \
    curl \
    vim \
    && apt-get clean

WORKDIR /xv6
