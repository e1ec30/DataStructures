FROM gcevans/cs225-container:fa22
ENTRYPOINT bash

RUN apt-get update && apt-get install -y \
    curl \
    xz-utils \
    git \
    python3-pip \
    nodejs \
    npm \
    ripgrep \
    fd-find \
    fzf \
    wget \
    software-properties-common \
    gdb \
    build-essential \
    unzip \
    && rm -rf /var/lib/apt/lists/*

VOLUME /root
RUN add-apt-repository ppa:neovim-ppa/unstable -y
RUN apt-get update
RUN apt-get install -y make gcc ripgrep unzip git xclip neovim

# Create Neovim directories
RUN mkdir -p /root/.config/nvim
RUN mkdir -p /root/.local/share/nvim
RUN mkdir -p /root/.cache/nvim

RUN git clone --depth 1 https://github.com/e1ec30/kickstart.nvim ~/.config/nvim
RUN rm -rf ~/.config/nvim/.git

