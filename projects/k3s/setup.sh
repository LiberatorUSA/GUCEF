#!/bin/bash

# show what we have running before we begin
sudo kubectl get all -A

# kill all containers/pods
sudo /usr/local/bin/k3s-killall.sh

# get rid of k3s itself
sudo /usr/local/bin/k3s-uninstall.sh

# get a new copy of k3s
sudo curl -sfL https://get.k3s.io | sh -

# check to see if we are back online
sudo kubectl get nodes -o wide

# download Helm 3
sudo curl -fsSL -o get_helm.sh https://raw.githubusercontent.com/helm/helm/master/scripts/get-helm-3
sudo chmod 700 get_helm.sh

# set up helm
sudo ./get_helm.sh

# confirm we are dealing with Helm 3
sudo helm version

# set often used repos and update
#sudo helm repo add stable https://charts.helm.sh/stable --kubeconfig /etc/rancher/k3s/k3s.yaml
sudo helm repo add bitnami https://charts.bitnami.com/bitnami --kubeconfig /etc/rancher/k3s/k3s.yaml
#sudo helm repo add grafana https://grafana.github.io/helm-charts --kubeconfig /etc/rancher/k3s/k3s.yaml
sudo helm repo add jetstack https://charts.jetstack.io --kubeconfig /etc/rancher/k3s/k3s.yaml
sudo helm repo add jenkins https://charts.jenkins.io --kubeconfig /etc/rancher/k3s/k3s.yaml
sudo helm repo add gitlab https://charts.gitlab.io/ --kubeconfig /etc/rancher/k3s/k3s.yaml
sudo helm repo add github-actions-runner-controller https://actions-runner-controller.github.io/actions-runner-controller --kubeconfig /etc/rancher/k3s/k3s.yaml
sudo helm repo update --kubeconfig /etc/rancher/k3s/k3s.yaml

# install the common prereq of cert manager
sudo helm install cert-manager jetstack/cert-manager --namespace cert-manager --create-namespace --set installCRDs=true --set prometheus.enabled=true --set webhook.timeoutSeconds=4 --kubeconfig /etc/rancher/k3s/k3s.yaml --debug

# setup GitHub runner integration
cd ./github
./setup.sh
cd ..






