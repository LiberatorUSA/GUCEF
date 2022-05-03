#!/bin/bash

# create a namespace to host self-hosted runners resources.
sudo kubectl create namespace github-runners

# Add repository
sudo helm repo add actions-runner-controller https://actions-runner-controller.github.io/actions-runner-controller --kubeconfig /etc/rancher/k3s/k3s.yaml
sudo helm repo update --kubeconfig /etc/rancher/k3s/k3s.yaml

# Install the runner controller. Note that the security token MUST be provided in the --values referenced file!
sudo helm install actions-runner-controller actions-runner-controller/actions-runner-controller --namespace github-runners --create-namespace --wait --timeout 30s --kubeconfig /etc/rancher/k3s/k3s.yaml --debug --values=./github_runner_auth.yaml

# Verify installation
sudo kubectl --namespace github-runners get all
