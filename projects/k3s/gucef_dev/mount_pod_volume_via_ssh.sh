#!/bin/bash

# Make sure we have sshfs installed. Its a must-have dependency for this to work
echo " --- "
echo "Making sure sshfs is installed..."
echo " --- "
sudo apt-get -y install sshfs

echo " --- "
echo "Creating mount dir 'pod_volume' as a local sub-dir..."
echo " --- "
mkdir -p ./pod_volume

echo " --- "
echo "Mounting FileSystem via SSH to ./pod_volume for the current user ..."
echo " --- "
sshfs -p 10022 root@localhost:/ ./pod_volume

