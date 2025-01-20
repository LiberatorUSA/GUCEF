#!/bin/bash

# build the github GUCEF codebase development image
docker build -f github_gucef_dev_base_image.dockerfile . -t vanvelzen/github-gucef-dev-base:latest

# save the image as a local tar file
docker save --output ./vanvelzen-github-gucef-dev-base-latest.tar vanvelzen/github-gucef-dev-base:latest

# Import the image into k3s
sudo k3s ctr images import ./vanvelzen-github-gucef-dev-base-latest.tar

