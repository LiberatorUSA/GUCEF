#!/bin/bash

# add helm repo needed to get Nexus helm chart
sudo helm repo add sonatype https://sonatype.github.io/helm3-charts/ --kubeconfig /etc/rancher/k3s/k3s.yaml
sudo helm repo update --kubeconfig /etc/rancher/k3s/k3s.yaml

sudo helm install my-nexus-repository-manager sonatype/nexus-repository-manager --namespace nexus --create-namespace --version 38.1.1 --kubeconfig /etc/rancher/k3s/k3s.yaml --debug

