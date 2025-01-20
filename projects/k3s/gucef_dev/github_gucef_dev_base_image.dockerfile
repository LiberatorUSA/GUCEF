# syntax=docker/dockerfile:1

# --------------------------------------------------------------------
# First lets use the 3rd party base image which deals with all the agent setup
# That alone however does not get us to the point of building our software
# we want the runner out-of-the-box to come with the needed build tools and dependencies
# --------------------------------------------------------------------

# install build tooling in base image
FROM ubuntu:24.04
RUN apt-get update && apt-get -y install apt-utils build-essential git-all clang clang-tools clang-tidy llvm cppcheck libssl-dev rpm libx11-dev graphviz doxygen cmake premake4 ssh gdb gdbserver openssh-server netcat-traditional grep gh

# Install the GUCEF codebase
RUN git clone https://github.com/DinandVanvelzen/GUCEF.git

# Expose the port for gdbserver
EXPOSE 4020

# --------------------------------------------------------------------
# For broad compatibility and access we will use a dedicated SSH server inside 
# the container as the main process
# --------------------------------------------------------------------

# Switch to root to perform file operations
USER root

RUN mkdir /var/run/sshd
# Set root password for SSH access (change 'admin' to your desired password)
RUN echo 'root:admin' | chpasswd
# changes the sshd_config file to enable root password login, which means allowing the "root" user to log in using a password instead of relying on other authentication methods like SSH keys.
RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config
# modifies the Pluggable Authentication Module configuration for the SSH daemon to prevent possible issues with systemd.
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd
# exposes port 22 in the container, allowing SSH connections to the SSH server running inside the container.
EXPOSE 22
# Add a script to check if the SSH port is open and report back
COPY check_ssh_port.sh /check_ssh_port.sh
RUN chmod +x /check_ssh_port.sh
# Add a script to start both the SSH server and the agent application 
COPY github_gucef_dev_base_entrypoint.sh /github_gucef_dev_base_entrypoint.sh 
RUN chmod +x /github_gucef_dev_base_entrypoint.sh

# Set the entrypoint to the start script we installed above
ENTRYPOINT ["/github_gucef_dev_base_entrypoint.sh"]
