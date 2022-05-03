#!/bin/bash

# create a namespace to host self-hosted runners resources.
sudo kubectl create namespace github-runners

# apply the Kubernetes manifest
sudo kubectl --namespace github-runners apply -f ./github-runner.yaml

# verify
sudo kubectl --namespace github-runners get runner
