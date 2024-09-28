#!/bin/bash

sudo kubectl get deployments -n devops-tools
sudo kubectl delete deployments/jenkins -n devops-tools
sudo kubectl get deployments -n devops-tools

sudo kubectl get services -n devops-tools
sudo kubectl delete services/jenkins -n devops-tools
sudo kubectl get services -n devops-tools

sudo kubectl get pods -n devops-tools
sudo kubectl delete all -n devops-tools --all
sudo kubectl get pods -n devops-tools

sudo kubectl get pvc -n devops-tools
sudo kubectl delete pvc jenkins-pv-claim -n devops-tools
sudo kubectl get pvc -n devops-tools

sudo kubectl get configmaps -n devops-tools
sudo kubectl delete configmap jenkins-casc -n devops-tools
sudo kubectl get configmaps -n devops-tools

# We use the dynamic provisioner now so the below should not be needed
#sudo kubectl get pv
#sudo kubectl delete pv jenkins-pv
#sudo kubectl get pv

sudo docker image remove jenkins/jenkins --force
sudo docker image remove jenkins/jenkins:lts --force
sudo docker image remove vanvelzen/jenkins-server-base --force
