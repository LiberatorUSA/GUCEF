#!/bin/bash

# install the pipe to GitHub
./github_access_runner_install.sh

# create the github runner image which is a derived image
./create_github_runner_image.sh

# install the github runner using the image created above
./github_runner_install.sh


