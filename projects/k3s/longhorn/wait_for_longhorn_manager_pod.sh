#!/bin/bash

# Set the namespace where longhorn manager is running
NAMESPACE="longhorn-system"

# Find the longhorn manager pod name by looking for the pod with the "longhorn-manager-" prefix
POD_NAME=$(sudo kubectl get pods -n $NAMESPACE --no-headers -o custom-columns=":metadata.name" | grep '^longhorn-manager-')

# Check if the pod was found
if [[ -z "$POD_NAME" ]]; then
  echo "longhorn-manager pod not found in namespace $NAMESPACE"
  exit 1
fi
echo "Found longhorn-manager pod: $POD_NAME"
export POD_NAME

echo "Waiting up to 60secs for the pod to be ready"
TIMEOUT=60 # Timeout in seconds
start_time=$(date +%s)
while true; do
  sudo kubectl get pod "$POD_NAME" -n "$NAMESPACE" | grep -q "Running"
  if [ $? -eq 0 ]; then
    echo "Pod $POD_NAME is ready!"
    break
  fi

  elapsed_time=$(($(date +%s) - $start_time))
  if [ $elapsed_time -gt $TIMEOUT ]; then
    echo "Timeout waiting for pod $POD_NAME to become ready."
    exit 1
  fi

  sleep 5
done
