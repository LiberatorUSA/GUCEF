#!/bin/bash

# Set the namespace where Jenkins is running
NAMESPACE="devops-tools"

# Set the file to extract
FILE_PATH="/var/jenkins_home/casc_configs/jenkins_config.yaml"

. ./wait_for_jenkins_pod.sh

# Use kubectl to copy the initialAdminPassword file from the Jenkins pod to the local machine
echo "Attempting to copy from: $NAMESPACE/$POD_NAME:$FILE_PATH"
sudo kubectl cp $NAMESPACE/$POD_NAME:$FILE_PATH ./jenkins_config_from_pod.yaml

# Check if the file was successfully copied and is non-empty
if [[ -f ./jenkins_config_from_pod.yaml && -s ./jenkins_config_from_pod.yaml ]]; then
  echo "File successfully copied to ./jenkins_config_from_pod.yaml"
else
  echo "Failed to copy the file."
  exit 1
fi
