#!/bin/bash

# Define the Longhorn namespace
NAMESPACE="longhorn-system"

# Delete Longhorn manager pods
sudo kubectl delete pods -n $NAMESPACE -l app.kubernetes.io/name=longhorn-manager

# Delete Longhorn instance manager pods
sudo kubectl delete pods -n $NAMESPACE -l app.kubernetes.io/name=longhorn-instance-manager

echo "Longhorn services restart commands executed"
