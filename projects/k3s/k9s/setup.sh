#!/bin/bash

# First make sure pacman is installed on the system
# Download the K9s Debian Package
wget https://github.com/derailed/k9s/releases/download/v0.32.7/k9s_linux_amd64.deb

# Install K9s
sudo apt install ./k9s_linux_amd64.deb

# Clean Up
rm ./k9s_linux_amd64.deb


