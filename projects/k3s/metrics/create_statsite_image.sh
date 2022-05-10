#!/bin/bash

# first get a listing of the current container for analysis
docker inspect statsite/statsite:latest

# get the source code
git clone https://github.com/statsite/statsite.git
cd ./statsite

# build the statsite image straight from the source code
docker build -t statsite/statsite:latest .

# remove the previous image export if any
rm ./statsite-statsite-latest.tar

# save the image as a local tar file
docker save --output ./statsite-statsite-latest.tar statsite/statsite:latest

# Import the image into k3s
sudo k3s ctr images import ./statsite-statsite-latest.tar

