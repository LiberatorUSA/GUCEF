# syntax=docker/dockerfile:1

# First create base image
FROM summerwind/actions-runner:latest as github-runner-base

# install build tooling in derived base image
FROM github-runner-base
RUN sudo apt-get update && sudo apt-get -y install apt-utils build-essential git-all libssl-dev rpm libx11-dev graphviz doxygen cmake premake4 ssh gdb gdbserver

