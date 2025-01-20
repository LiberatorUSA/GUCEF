#!/bin/bash

sudo kubectl create namespace gucef

# Create the storage volume for the codebase
sudo kubectl apply -f ./code-volume.yaml -n gucef

# Deploy into k8s
sudo kubectl apply -f ./deployment.yaml -n gucef

# Setup service for external convenient access into the dev space
sudo kubectl apply -f ./service.yaml -n gucef
sudo kubectl get services -n gucef


