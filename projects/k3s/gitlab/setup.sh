#!/bin/bash

# create a namespace to host gitlabresources.
sudo kubectl create namespace gitlab

# add helm repo needed to get GitLab helm chart
sudo helm repo add gitlab http://charts.gitlab.io/ --kubeconfig /etc/rancher/k3s/k3s.yaml
sudo helm repo update --kubeconfig /etc/rancher/k3s/k3s.yaml

sudo helm uninstall my-gitlab --namespace gitlab --kubeconfig /etc/rancher/k3s/k3s.yaml --debug
sudo helm install my-gitlab gitlab/gitlab --version 5.10.2 --namespace gitlab --create-namespace --kubeconfig /etc/rancher/k3s/k3s.yaml --debug --values=./values.yaml
# --version 38.1.1

# Verify installation
sudo kubectl --namespace gitlab get all
