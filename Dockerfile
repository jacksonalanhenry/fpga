# .devcontainer/Dockerfile
FROM docker-external.d16.us.swri.org/ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && apt install -y \
  verilator \
  build-essential \
  bear \
  gtkwave \
  git \
  curl \
  vim neovim \
  python3 python3-pip \
  libx11-dev libxft-dev libxext-dev \
  libfftw3-dev \
  xauth x11-apps \
  gosu \
  && apt clean

RUN apt install -y software-properties-common && \
    add-apt-repository ppa:neovim-ppa/unstable -y && \
    apt update && \
    apt install -y neovim

COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
CMD ["/bin/bash"]
