#!/bin/bash

# Check if the pod name is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <pod-name> [namespace]"
  exit 1
fi

POD_NAME=$1
NAMESPACE=${2:-default} # Default to 'default' namespace if not provided

# Get the IP address of the pod
POD_IP=$(sudo kubectl get pod $POD_NAME -n $NAMESPACE -o jsonpath='{.status.podIP}')

# Check if the command was successful
if [ $? -ne 0 ]; then
  echo "Failed to get the IP address of the pod in namespace $NAMESPACE."
  exit 1
fi

# Check if the IP address was found
if [ -z "$POD_IP" ]; then
  echo "No IP address found for pod $POD_NAME in namespace $NAMESPACE."
  exit 1
fi

echo "The IP address of the pod $POD_NAME in namespace $NAMESPACE is: $POD_IP"

# Store the result in a variable (outside the script)
export POD_IP
