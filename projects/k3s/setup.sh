#!/bin/bash

# show what we have running before we begin
sudo kubectl get all -A

# kill all containers/pods
sudo /usr/local/bin/k3s-killall.sh

# get rid of k3s itself
sudo /usr/local/bin/k3s-uninstall.sh

# get a new copy of k3s
sudo curl -sfL https://get.k3s.io | sh -

#set the location of the kubeconfig
export KUBECONFIG=/etc/rancher/k3s/k3s.yaml

# check to see if we are back online
sudo kubectl get nodes -o wide

# download Helm 3
sudo curl -fsSL -o get_helm.sh https://raw.githubusercontent.com/helm/helm/master/scripts/get-helm-3
sudo chmod 700 get_helm.sh

# set up helm
sudo ./get_helm.sh
rm -f ./get_helm.sh

# confirm we are dealing with Helm 3
sudo helm version

# set often used repos and update
sudo helm repo add bitnami https://charts.bitnami.com/bitnami --kubeconfig /etc/rancher/k3s/k3s.yaml
sudo helm repo add jenkins https://charts.jenkins.io --kubeconfig /etc/rancher/k3s/k3s.yaml
sudo helm repo add gitlab https://charts.gitlab.io/ --kubeconfig /etc/rancher/k3s/k3s.yaml
sudo helm repo update --kubeconfig /etc/rancher/k3s/k3s.yaml

# install the local path provisioner, which is a plugin for k3s
# it comes standard with k3s as provisioner "rancher.io/local-path"
# you can update it independently here (since its a newer feature and you might want latest)
# stable version:
sudo kubectl apply -f https://raw.githubusercontent.com/rancher/local-path-provisioner/v0.0.29/deploy/local-path-storage.yaml
# latest version:
#sudo kubectl apply -f https://raw.githubusercontent.com/rancher/local-path-provisioner/master/deploy/local-path-storage.yaml

# Make the local drives of the node available to k8s
# comment this line or edit the yaml for your own node
cd ./host-node
./setup.sh
cd ..

# install the common prereq of cert manager
cd ./cert-manager
./setup.sh
cd ..

# setup the Kubernetes dashboard
cd ./dashboard
./setup.sh
cd ..

# setup MySQL
cd ./mysql
./setup.sh
cd ..

# setup GitHub runner integration
cd ./github
./setup.sh
cd ..

# setup Nexus for storing build artifacts and deployment, CICD
cd ./nexus
./setup.sh
cd ..

# setup Jenkins for CICD
cd ./jenkins
./setup.sh
cd ..

# setup metrics support
cd ./metrics
./setup.sh
cd ..

# Make the kubeconfig available for our home kubectrl
sudo mkdir -p ~/.kube/
sudo cp -f -r /etc/rancher/k3s/k3s.yaml ~/.kube/
sudo mv ~/.kube/k3s.yaml ~/.kube/config
sudo chmod a+rw ~/.kube/config

