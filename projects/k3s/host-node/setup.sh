#!/bin/bash

# Generate a persistent volume yaml for all drives on the host node
rm -fv ./persistent-volumes.yaml
./generate_pv_yaml.sh

# Make the local drives of the node available to k8s
# comment this line or edit the yaml for your own node if needed
sudo kubectl apply -f persistent-volumes.yaml
