#!/bin/bash

# print which storage classes the cluster already knows about
sudo kubectl get storageclass

# publish our storage classes into the cluster
# we add the concept of a "raid" and "jbod" storage class
# these might be used to reference similarly named PVs based on mounts
sudo kubectl apply -f ./storage-classes.yaml

# print which storage classes the cluster now knows about
sudo kubectl get storageclass

# Generate a persistent volume yaml for all mounted drives on the host node using the mount names
# Note that "raid" and "mirror" are reserved strings which will be auto set to StorageClass "raid"
# Note that "jbod" is a reserved string which will be auto set to StorageClass "raid"
rm -fv ./persistent-volumes.yaml
./generate_pv_yaml.sh

# Make the local drives of the node available to k8s
# comment this line or edit the yaml for your own node if needed
sudo kubectl apply -f persistent-volumes.yaml
