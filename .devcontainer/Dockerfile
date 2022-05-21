ARG IMAGE
ARG TAG
FROM ${IMAGE}:${TAG}

# Install system packages.
RUN rm -f /etc/apt/apt.conf.d/docker-clean
COPY apt.txt /tmp/apt.txt
RUN --mount=type=cache,target=/var/cache/apt apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y $(cat /tmp/apt.txt)

# Create a default non-root user.
ARG USER=developer
ARG UID=1000
ARG GID=1000
RUN groupadd --gid=${GID} ${USER}
RUN useradd --uid=${UID} --gid=${GID} --shell=/bin/bash --create-home --groups=sudo ${USER}

# Enable direnv for all users.
RUN echo 'eval "$(direnv hook bash)"' >> /etc/bash.bashrc

# Don't prompt for passwords when using sudo.
RUN echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers
