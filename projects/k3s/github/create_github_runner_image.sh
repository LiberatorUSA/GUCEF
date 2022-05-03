#!/bin/bash

#if [ $# -eq 0 ]
#  then
#    tag='latest'
#  else
#    tag=$1
#fi


# first get a listing of the current container for analysis
docker inspect summerwind/actions-runner:latest

# build the derived github runner build image
docker build -f github_runner_build_image.dockerfile . -t vanvelzen/github-runner-base:latest

# save the image as a local tar file
docker save --output ./vanvelzen-github-runner-base-latest.tar vanvelzen/github-runner-base:latest

# Import the image into k3s
sudo k3s ctr images import ./vanvelzen-github-runner-base-latest.tar

