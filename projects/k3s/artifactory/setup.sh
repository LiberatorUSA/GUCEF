#!/bin/bash

# we need the jfrog repo to get the goodies
sudo helm repo add jfrog https://charts.jfrog.io/ --kubeconfig /etc/rancher/k3s/k3s.yaml
sudo helm repo update --kubeconfig /etc/rancher/k3s/k3s.yaml

# uninstall if we already had it installed
sudo helm uninstall artifactory && sleep 90 && kubectl delete pvc -l app=artifactory

# delete artifactory which will ALSO DELETE DATA VOLUMES
#sudo helm delete artifactory --namespace artifactory

# proceed with the install
sudo helm install artifactory jfrog/artifactory-oss --namespace artifactory --create-namespace --set installCRDs=true --kubeconfig /etc/rancher/k3s/k3s.yaml --debug

