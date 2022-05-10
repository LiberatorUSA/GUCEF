#!/bin/bash

# create a namespace to host metrics resources.
sudo kubectl create namespace metrics

# install Graphite
sudo kubectl create -f ./graphite-deployment.yml -n metrics

# create the statsite image from source and make it available to use
./create_statsite_image.sh

# install statsite
sudo kubectl create -f ./statsite-deployment.yml -n metrics

# install Grafana
sudo kubectl create -f ./grafana-deployment.yml -n metrics

# Now, we will run Snap Daemon in our Kubernetes cluster. 
# Snap Daemon is the monitoring daemon which will pull various kubernetes monitoring metrics and push them into Graphite.
sudo kubectl create -f ./snap_ds.yml -n metrics

