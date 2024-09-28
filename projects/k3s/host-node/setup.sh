#!/bin/bash

# print which storage classes the cluster already knows about
sudo kubectl get storageclass

# publish our storage classes into the cluster
# we add the concept of a "raid" and "jbod" storage class
# these might be used to reference similarly named PVs based on mounts
echo "Updating the StorageClass definitions in the cluster..."
sudo kubectl apply -f ./storage-classes.yaml

# print which storage classes the cluster now knows about
sudo kubectl get storageclass

# the following step needs to perform some JSON parsing on the command line for which we use the tool jq
# lets make sure its installed: Ubuntu/Debian:
sudo apt-get install jq -y

# Map the StorageClass to specific /mnt/ paths based on naming convention for all mounted drives on the host node using the mount names
# Note that "raid" and "mirror" are reserved strings which will be auto set to StorageClass "raid"
# Note that "jbod" is a reserved string which will be auto set to StorageClass "raid"
rm -f ./local-path-config-backup.json
rm -f ./local-path-config-updated.json
./generate-updated-provisioner-config.sh

# Automatically update the ConfigMap in the cluster
# Comment below if you want to review it first before directly applying the config to the cluster
echo "Updating the ConfigMap in the cluster..."
sudo kubectl patch configmap local-path-config -n kube-system --type merge --patch "$(jq -Rs '{data: {"config.json": .}}' < ./local-path-config-updated.json)"
