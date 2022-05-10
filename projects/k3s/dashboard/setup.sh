#!/bin/bash

# first delete the dashboard if one already exists
sudo k3s kubectl delete ns kubernetes-dashboard
sudo k3s kubectl delete clusterrolebinding kubernetes-dashboard
sudo k3s kubectl delete clusterrole kubernetes-dashboard

GITHUB_URL=https://github.com/kubernetes/dashboard/releases
VERSION_KUBE_DASHBOARD=$(curl -w '%{url_effective}' -I -L -s -S ${GITHUB_URL}/latest -o /dev/null | sed -e 's|.*/||')
sudo k3s kubectl create -f https://raw.githubusercontent.com/kubernetes/dashboard/${VERSION_KUBE_DASHBOARD}/aio/deploy/recommended.yaml

# Deploy the admin-user configuration:
sudo k3s kubectl create -f ./dashboard.admin-user.yml -f ./dashboard.admin-user-role.yml

# Obtain the Bearer Token
sudo k3s kubectl -n kubernetes-dashboard describe secret admin-user-token | grep '^token'

# To access the Dashboard you must create a secure channel to your K3s cluster:
#sudo k3s kubectl proxy

# The Dashboard is now accessible at:
#  http://localhost:8001/api/v1/namespaces/kubernetes-dashboard/services/https:kubernetes-dashboard:/proxy/
#  Sign In with the admin-user Bearer Token

