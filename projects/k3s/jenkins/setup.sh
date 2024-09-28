#!/bin/bash

# print the current list of resources
sudo kubectl get services -n devops-tools
sudo kubectl get pvc -n devops-tools
sudo kubectl get pods -n devops-tools

# create the jenkins server image we wil use for our pod
# this is imported to the k3s installation and made available as a known image
./create_jenkins_server_image.sh
rm -f ./vanvelzen-jenkins-server-base-latest.tar

# create a namespace to host Jenkins resources.
sudo kubectl create namespace devops-tools

sudo kubectl create configmap jenkins-casc --from-file=./jenkins_config.yaml -n devops-tools

sudo kubectl apply -f jenkins-volume.yaml -n devops-tools
sudo kubectl apply -f jenkins-serviceAccount.yaml -n devops-tools
sudo kubectl apply -f jenkins-service.yaml -n devops-tools
sudo kubectl apply -f jenkins-deployment.yaml -n devops-tools

# extract the config from the pod so we can check it
rm -f ./jenkins_config_from_pod.yaml
./extract_jenkins_config.sh

# extract the admin password from the pod so we can check it
rm -f ./initialAdminPassword
./extract_initial_admin_password.sh

# print the list of resources again after the above
sudo kubectl get services -n devops-tools
sudo kubectl get pvc -n devops-tools
sudo kubectl get pods -n devops-tools
