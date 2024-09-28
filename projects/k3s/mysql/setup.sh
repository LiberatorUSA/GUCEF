#!/bin/bash

# print the current services
sudo kubectl get services
sudo kubectl get services -n mysql

# create a namespace to host metrics resources.
sudo kubectl create namespace mysql

sudo kubectl apply -f mysql-pvc.yaml -n mysql
sudo kubectl apply -f mysql-statefulset.yaml -n mysql
sudo kubectl apply -f mysql-service.yaml -n mysql

# print the list of services again after the above
sudo kubectl get services
sudo kubectl get services -n mysql
