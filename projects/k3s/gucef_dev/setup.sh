#!/bin/bash

# create the image using docker and import it to k3s
./create_github_gucef_dev_base_image.sh

# deploy a container running the image we created above
./deploy_github_gucef_dev_base.sh
